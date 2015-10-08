/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   cgi_recover.c

 Description: we recover a backup version from the server

 Author:      hexiong

 Email:       hexiong@tp-link.net

--------------------------------------

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../socket_h/cgic_client.h"
#include "../socket_h/message.h"

#define MAXLINE 4096  /* the maxline of the buffer */
#define SERV_PORT 3000  /* port number */
#define RECOVER_MARK_LEN 2 /* the length of the check message mark */
#define FILE_BUFFER_LEN 4000 /* the length of the file buffer */

int cgi_recover(int id, char *IP)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char file_buffer[FILE_BUFFER_LEN];
    char sendline[MAXLINE], recvline[MAXLINE];

    /* initialize the send buffer and receive buffer */
    memset(&sendline, 0, MAXLINE);
    memset(&sendline, 0, MAXLINE);

    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0)
    {
        perror("cgi_recover:Problem in creating the socket\n");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP);
    /* convert to big-endian order */
    servaddr.sin_port = htons(SERV_PORT);

    /* connection of the client to the socket */
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        perror("cgi_recover:Problem in connecting to the server");
        exit(2);
    }

    strncpy(sendline, CGI_RECOVER, RECOVER_MARK_LEN);
    memcpy(&sendline[RECOVER_MARK_LEN],&id, sizeof(int));
    send(sockfd, sendline, strlen(sendline), 0);

    while (1)
    {
        printf("we reach here or not\n");
        if (recv(sockfd, recvline, MAXLINE, 0) < 0)
        {
            perror("cgi_recover:The server terminated prematurely\n");
            return -1;
        }
        else
        {
            /* server response this request */
            if (strncmp(recvline, RECOVER_RESPONSE, RECOVER_MARK_LEN) == 0)
            {
                printf("The recvline is %s\n", recvline);
                printf("the first recvline length is %d\n", strlen(recvline));
                char file_path[512] = "/home/luckybear/Documents/test.tar";
                FILE *fp = fopen(file_path, "ab");
                struct data_transfer *data = malloc(sizeof(*data));
                int length = 0;
                memset(recvline, 0, MAXLINE);
                memset(file_buffer, 0, FILE_BUFFER_LEN);
                while ((length = recv(sockfd, recvline, MAXLINE, 0)) > 0)
                {
                    //printf("1The length is %d\n",strlen(recvline));
                    //printf("The recvline is %s\n", recvline);
                    if (recvline[0] == '1')
                    {
                        memcpy(data, &recvline[1], sizeof(*data));
                        printf("The file length we will write is %s\nthe length is %d\n",file_buffer, length);
                        if (fwrite(data->buffer, 1, data->length, fp) < 0)
                        {
                            printf("File Write failed.\n");
                            break;
                        }
                        memset(recvline, 0, MAXLINE);
                        memset(data->buffer, 0, FILE_BUFFER_SIZE);
                        memset(sendline, 0, MAXLINE);
                        printf("we reach here to send something\n");
                        sendline[0] = '1';
                        printf("the sendline is %s\n", sendline);
                        send(sockfd, sendline, MAXLINE, 0);
                    }
                    else
                    {
                        break;
                    }
                }
                if(length < 0)
                {
                    return -1;
                }
                else
                {
                    printf("we reach here?\n");
                    fclose(fp);
                    return 1;
                }
            }
            else
            {
                /* server doesn't response this request */
                printf("we reach here and exit the program\n");
                return -1;
            }
        }
    }
}

int main()
{
    cgi_recover(20,"127.0.0.1");
    return 0;
}