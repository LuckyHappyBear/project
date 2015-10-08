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

int cgi_backup(char *IMSI, char *IP, char *product_id, char *note, char *file_path)
{
    int sockfd;
    int start_pos;          /* count every field's start position */
    int return_code;
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
    get_version_no(version_no);
    strncpy(&sendline[start_pos], version_no, VERSION_NUM_LEN);

    /* note field */
    start_pos += VERSION_NUM_LEN;
    strncpy(&sendline[start_pos], note, strlen(note));

    /* field length field */
    /*start_pos += MAX_NOTE_LEN;
    strncpy(&sendline[start_pos], file_size, strlen(file_size));*/
    send(sockfd, sendline, strlen(sendline), 0);
    #if CGI_TEST
    printf("the sendline is %s\n",sendline);
    #endif
    memset(sendline, 0, MAXLINE);
    //printf("the length is %s\n", &sendline[start_pos]);
    file_path = "/home/luckybear/alpha.tar";
    while (1)
    {
        if (recv(sockfd, recvline, MAXLINE, 0) > 0)
        {
            if (strncmp(recvline, BACKUP_RESPONSE, BACKUP_MARK_LEN) == 0)
            {
                memset(sendline, 0, MAXLINE);
                strncpy(sendline, CGI_BACKUP, BACKUP_MARK_LEN);
                sendline[2] = '1';
                #if CGI_TEST
                printf("The sendline is %s\n", sendline);
                #endif
                send(sockfd, sendline, strlen(sendline), 0);
                memset(sendline, 0, MAXLINE);
                memset(recvline, 0, MAXLINE);

                /* wait for response */
                while (recv(sockfd, recvline, MAXLINE, 0) > 0)
                {
                    #if CGI_TEST
                    printf("did we reveice the response\n");
                    printf("The recvbuf content is %s\n",recvline);
                    #endif
                    if(recvline[0] == '1')
                    {
                        #if CGI_TEST
                        printf("we receive client's response\n");
                        #endif
                        break;
                    }
                }

                FILE *fp = fopen(file_path,"rb");
                struct data_transfer *data = malloc(sizeof(*data));
                if(NULL == fp)
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
                    fseek(fp, 0, SEEK_SET);

                    int length = 0;
                    /* the length we have read */
                    int read_len = 0;

                    /* the length we have not read */
                    int left_len = total_bytes;

                    memset(sendline, 0, MAXLINE);
                    while (1)
                    {
                        if ((left_len <= 0) || (read_len >= total_bytes))
                        {
                            fclose(fp);
                            memset(sendline, 0, MAXLINE);
                            sendline[0] = '0';
                            sendline[1] = '\0';
                            send(sockfd, sendline, strlen(sendline), 0);
                            memset(sendline, 0, MAXLINE);
                            #if CGI_TEST
                            printf("we finished here\n");
                            #endif
                            break;
                        }

                        if (left_len >= FILE_BUFFER_SIZE)
                        {
                            memset(data->buffer, 0, FILE_BUFFER_SIZE);
                            length = fread(data->buffer, 1,
                                           FILE_BUFFER_SIZE, fp);
                            data->length = length;
                            #if CGI_TEST
                            printf("The length in there is  %d\n", length);
                            #endif
                            read_len += length;
                            #if CGI_TEST
                            printf("The read_len is %d\n",read_len);
                            #endif
                        }
                        else
                        {
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

                        sendline[0] = '1';
                        memcpy(&sendline[1], data, sizeof(*data));
                        #if CGI_TEST
                        printf("The length is %d\n",strlen(sendline));
                        printf("The sendbuffer is %s\n", sendline);
                        #endif

                        if (send(sockfd, sendline, sizeof(*data) + 1, 0) < 0)
                        {
                            #if CGI_TEST
                            perror("Send File Failed");
                            #endif
                            return -1;
                        }
                        else
                        {
                            #if CGI_TEST
                            printf("we reach here to wait for response\n");
                            #endif
                            memset(recvline, 0, MAXLINE);
                            while (recv(sockfd, recvline, MAXLINE, 0) > 0)
                            {
                                #if CGI_TEST
                                printf("did we reveice the response\n");
                                printf("The recvbuf content is %s\n",recvline);
                                #endif
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
            #if CGI_TEST
            perror("cgi_backup:The server terminated prematurely\n");
            #endif
            return -1;
        }
    }
}

