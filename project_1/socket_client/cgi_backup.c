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
#include "../socket_h/cgi.h"

#define MAXLINE 4096  /* the maxline of the buffer */
#define SERV_PORT 3000  /* port number */
#define BACKUP_MARK_LEN 2 /* the length of the check message mark */
#define FILE_BUFFER_LEN 4000 /* the size of the file buffer */

int cgi_backup(char *IMSI, char *IP, char *product_id, char *note, char *file_path)
{
    int sockfd;
    int start_pos;          /* count every field's start position */
    int return_code;
    int file_len;
    char file_size[5];
    char version_no[14];
    char file_buffer[4000];
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    /* initialize the send buffer and receive buffer */
    memset(&sendline, 0, MAXLINE);
    memset(&sendline, 0, MAXLINE);

    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0)
    {
        perror("cgi_backup:Problem in creating the socket\n");
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
        perror("cgi_backup:Problem in connecting to the server");
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
    printf("the sendline is %s\n",sendline);
    memset(sendline, 0, MAXLINE);
    //printf("the length is %s\n", &sendline[start_pos]);
    file_path = "/home/luckybear/alpha.tar";
    while (1)
    {
        printf("we reach here or not\n");
        if (recv(sockfd, recvline, MAXLINE, 0) > 0)
        {
            if (strncmp(recvline, BACKUP_RESPONSE, BACKUP_MARK_LEN) == 0)
            {
                memset(sendline, 0, MAXLINE);
                strncpy(sendline, CGI_BACKUP, BACKUP_MARK_LEN);
                sendline[2] = '1';
                printf("The sendline is %s\n", sendline);
                send(sockfd, sendline, strlen(sendline), 0);
                memset(sendline, 0, MAXLINE);
                memset(recvline, 0, MAXLINE);

                /* wait for response */
                while (recv(sockfd, recvline, MAXLINE, 0) > 0)
                {
                    printf("did we reveice the response\n");
                    printf("The recvbuf content is %s\n",recvline);
                    if(recvline[0] == '1')
                    {
                        printf("we receive client's response\n");
                        break;
                    }
                }

                FILE *fp = fopen(file_path,"rb");
                if(NULL == fp)
                {
                    printf("File Not Found\n");
                }
                else
                {
                    /* get the total length of the file */
                    fseek(fp, 0, SEEK_SET);
                    fseek(fp, 0, SEEK_END);
                    long total_bytes = ftell(fp);
                    printf("The total length of the file is %ld\n",total_bytes);
                    fseek(fp, 0, SEEK_SET);

                    int length = 0;
                    /* the length we have read */
                    int read_len = 0;

                    /* the length we have not read */
                    int left_len = total_bytes;

                    memset(file_buffer, 0, FILE_BUFFER_LEN);
                    memset(sendline, 0, MAXLINE);
                    while (1)
                    {
                        if ((left_len <= 0) || (read_len >= total_bytes))
                        {
                            break;
                        }

                        if ((length = fread(file_buffer, sizeof(char),
                                              FILE_BUFFER_LEN - 1, fp)) > 0)
                        {
                            sendline[0] = '1';
                            strncpy(&sendline[1], file_buffer, length);
                            printf("The length is %d\n",strlen(sendline));
                            printf("The sendbuffer is %s\n", sendline);
                            if (send(sockfd, sendline, strlen(sendline), 0) < 0)
                            {
                                perror("Send File Failed");
                                break;
                            }
                            else
                            {
                                printf("we reach here to wait for response\n");
                                memset(recvline, 0, MAXLINE);
                                while (recv(sockfd, recvline, MAXLINE, 0) > 0)
                                {
                                    printf("did we reveice the response\n");
                                    printf("The recvbuf content is %s\n",recvline);
                                    if(recvline[0] == '1')
                                    {
                                        printf("we receive client's response\n");
                                        break;
                                    }
                                }
                            }
                            printf("we continue send here\n");
                            memset(file_buffer, 0,FILE_BUFFER_LEN);
                            memset(sendline, 0, MAXLINE);
                        }
                    }
                    fclose(fp);
                    memset(sendline, 0, MAXLINE);
                    sendline[0] = '0';
                    sendline[1] = '\0';
                    send(sockfd, sendline, strlen(sendline), 0);
                    memset(sendline, 0, MAXLINE);
                    printf("we finished here\n");
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
            perror("cgi_backup:The server terminated prematurely\n");
            exit(3);
        }
    }
}

int main()
{
    cgi_backup("123456789871111","127.0.0.1","73000001","the first backup","/home/luckybear/test/testTime.c");
    return 0;
}
