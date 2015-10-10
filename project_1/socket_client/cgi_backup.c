/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   cgi_backup.c

 Description: this file use to complete cgi_backup function

 Author:      hexiong

 History:     hexiong create and complete the file

 --------------------------------------

 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../socket_h/message.h"
#include "../socket_h/public_handle.h"
#include "../socket_h/cgic_client.h"

#define MAXLINE 4096  /* the maxline of the buffer */
#define SERV_PORT 3000  /* port number */
#define BACKUP_MARK_LEN 2 /* the length of the check message mark */
#define FILE_BUFFER_LEN 1024 /* the size of the file buffer */

/******************************************************************************
 Function:     cgi_backup
 Description:  backup a local config to server
 Input:        IMSI:International Mobile Subscriber Identification Number
               IP:the server IP
               product_id: the id of the product
               note: the note user input
               file_path: the path of the config
 Output:       NONE
 Return:       1:successfule -1:failed
 Others:       NONE
*******************************************************************************/
int cgi_backup(char *IMSI, char *IP, char *product_id, char *note, char *file_path)
{
    int sockfd;
    int start_pos;          /* count every field's start position */
    char version_no[14];
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    /* initialize the send buffer and receive buffer */
    memset(&sendline, 0, MAXLINE);
    memset(&sendline, 0, MAXLINE);

    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0)
    {
        #if CGI_TEST
        perror("cgi_backup:Problem in creating the socket\n");
        #endif
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP);
    /* convert to big-endian order */
    servaddr.sin_port = htons(SERV_PORT);

    /* connection of the client to the socket */
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        #if CGI_TEST
        perror("cgi_backup:Problem in connecting to the server");
        #endif
        return -1;
    }

    /* information mark */
    strncpy(sendline, CGI_BACKUP, BACKUP_MARK_LEN);

    /* the third bit used to ditinguish data package or information package */
    sendline[2] = '0';

    /* IMSI field */
    strncpy(&sendline[BACKUP_MARK_LEN + 1], IMSI, strlen(IMSI));

    /* product_id field */
    start_pos = BACKUP_MARK_LEN + 1 + IMSI_LEN;
    strncpy(&sendline[start_pos], product_id, strlen(product_id));

    /* version_no field */
    start_pos += PRODUCT_ID_LEN;
    /*get_version_no(version_no);
    strncpy(&sendline[start_pos], version_no, VERSION_NUM_LEN);*/

    /* note field */
    //start_pos += VERSION_NUM_LEN;
    strncpy(&sendline[start_pos], note, strlen(note));

    /* send the buffer and then clear it */
    send(sockfd, sendline, start_pos + strlen(note), 0);
    memset(sendline, 0, MAXLINE);

    #if CGI_TEST
    printf("the sendline is %s\n",sendline);
    #endif

    while (1)
    {
        if (recv(sockfd, recvline, MAXLINE, 0) > 0)
        {
            /* server receive the backup request and response */
            if (strncmp(recvline, BACKUP_RESPONSE, BACKUP_MARK_LEN) == 0)
            {
                /* send request of sending data */
                memset(sendline, 0, MAXLINE);

                /* send request that client will send data */
                strncpy(sendline, CGI_BACKUP, BACKUP_MARK_LEN);
                sendline[2] = '1';
                sendline[3] = '\0';
                send(sockfd, sendline, strlen(sendline), 0);

                memset(sendline, 0, MAXLINE);
                memset(recvline, 0, MAXLINE);

                #if CGI_TEST
                printf("The sendline is %s\n", sendline);
                #endif

                /* wait for response from server */
                while (recv(sockfd, recvline, MAXLINE, 0) > 0)
                {
                    #if CGI_TEST
                    printf("did we reveice the response\n");
                    printf("The recvbuf content is %s\n",recvline);
                    #endif

                    /* get the response of the server */
                    if (recvline[0] == '1')
                    {
                        #if CGI_TEST
                        printf("we receive client's response\n");
                        #endif
                        break;
                    }
                }

                /* file operation */
                FILE *fp = fopen(file_path,"rb");
                struct data_transfer *data = malloc(sizeof(*data));
                /* the file didn't exist */
                if (NULL == fp)
                {
                    #if CGI_TEST
                    printf("File Not Found\n");
                    #endif
                    return -1;
                }
                else
                {
                    /* get the total length of the file */
                    fseek(fp, 0, SEEK_SET);
                    fseek(fp, 0, SEEK_END);
                    long total_bytes = ftell(fp);

                    #if CGI_TEST
                    printf("The total length of the file is %ld\n",total_bytes);
                    #endif

                    /* reset the file pointer */
                    fseek(fp, 0, SEEK_SET);

                    int length = 0;
                    /* the length we have read */
                    int read_len = 0;

                    /* the length we have not read */
                    int left_len = total_bytes;

                    memset(sendline, 0, MAXLINE);
                    while (1)
                    {
                        /* we have transfer all content */
                        if ((left_len <= 0) || (read_len >= total_bytes))
                        {
                            /* close the file and send the finish message */
                            fclose(fp);
                            memset(sendline, 0, MAXLINE);
                            sendline[0] = '0';
                            sendline[1] = '\0';
                            send(sockfd, sendline, 2, 0);

                            memset(sendline, 0, MAXLINE);
                            memset(recvline, 0, MAXLINE);

                            #if CGI_TEST
                            printf("we finished here\n");
                            #endif

                            /* wait for server's response */
                            while (recv(sockfd, recvline, MAXLINE, 0) > 0)
                            {
                                /* server receive successful */
                                if (recvline[0] == 'A')
                                {
                                    #if CGI_TEST
                                    printf("transfer successful\n");
                                    #endif
                                    free(data);
                                    return 1;
                                }
                                else
                                {
                                    free(data);
                                    return -1;
                                }

                            }
                        }

                        /* the left file length larger than buffer length */
                        if (left_len >= FILE_BUFFER_SIZE)
                        {
                            /* read and pad data */
                            memset(data->buffer, 0, FILE_BUFFER_SIZE);
                            length = fread(data->buffer, 1,
                                           FILE_BUFFER_SIZE, fp);
                            data->length = length;
                            read_len += length;

                            #if CGI_TEST
                            printf("The length in there is  %d\n", length);
                            printf("The read_len is %d\n",read_len);
                            #endif
                        }
                        else
                        {
                            /* pad left content in data */
                            memset(data->buffer, 0, FILE_BUFFER_SIZE);
                            length = fread(data->buffer, 1, left_len, fp);

                            #if CGI_TEST
                            printf("The length in there is  %d\n", length);
                            #endif

                            data->length = length;
                            read_len += length;

                            #if CGI_TEST
                            printf("The read_len is %d\n",read_len);
                            #endif
                        }

                        left_len = total_bytes - read_len;
                        /* pad sendline */
                        sendline[0] = '1';
                        memcpy(&sendline[1], data, sizeof(*data));

                        #if CGI_TEST
                        printf("The length is %d\n",strlen(sendline));
                        printf("The sendbuffer is %s\n", sendline);
                        #endif

                        /* send the data to server */
                        if (send(sockfd, sendline, sizeof(*data) + 1, 0) < 0)
                        {
                            #if CGI_TEST
                            perror("Send File Failed");
                            #endif
                            free(data);
                            return -1;
                        }
                        else
                        {
                            #if CGI_TEST
                            printf("we reach here to wait for response\n");
                            #endif
                            /* wait for server's response */
                            memset(recvline, 0, MAXLINE);
                            while (recv(sockfd, recvline, MAXLINE, 0) > 0)
                            {
                                #if CGI_TEST
                                printf("did we reveice the response\n");
                                printf("The recvbuf content is %s\n",recvline);
                                #endif

                                /* receive the right message from server */
                                if(recvline[0] == '1')
                                {
                                    #if CGI_TEST
                                    printf("we receive client's response\n");
                                    #endif
                                    break;
                                }
                            }
                        }
                        #if CGI_TEST
                        printf("we continue send here\n");
                        #endif
                        memset(sendline, 0, MAXLINE);
                    }
                }
                free(data);
                return 1;
            }
            else
            {
                /* server doesn't response this request */
                memset(&sendline, 0, MAXLINE);
                memset(&recvline, 0, MAXLINE);
                return -1;
            }
        }
        else
        {
            /* receive error */
            #if CGI_TEST
            perror("cgi_backup:The server terminated prematurely\n");
            #endif
            return -1;
        }
    }
}

