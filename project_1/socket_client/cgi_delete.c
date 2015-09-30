/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   cgi_delete.c

 Description: we delete backup version user choosed in this file

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
#include "../socket_h/cgi.h"
#include "../socket_h/message.h"

#define MAXLINE 4096  /* the maxline of the buffer */
#define SERV_PORT 3000  /* port number */
#define DELETE_MARK_LEN 2 /* the length of the check message mark */

int cgi_delete(int id, char *IP)
{
    int sockfd;
    int return_code;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    /* initialize the send buffer and receive buffer */
    memset(&sendline, 0, MAXLINE);
    memset(&sendline, 0, MAXLINE);

    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0)
    {
        perror("cgi_delete:Problem in creating the socket\n");
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
        perror("cgi_delete:Problem in connecting to the server");
        exit(2);
    }

    /* send delete message, request to delete backup version which id is id*/
    strncpy(sendline, CGI_DELETE, DELETE_MARK_LEN);
    memcpy(&sendline[DELETE_MARK_LEN], &id, sizeof(int));
    send(sockfd, sendline, strlen(sendline), 0);

    while (1)
    {
        if (recv(sockfd, recvline, MAXLINE, 0) < 0)
        {
            perror("cgi_delete:The server terminated prematurely\n");
            return -1;
        }
        else
        {
            /* server response this request */
            if (strncmp(recvline, DELETE_RESPONSE, DELETE_MARK_LEN) == 0)
            {
                if (recvline[2] == '1')
                {
                    /* delete successful */
                    printf("delete successful\n");
                    return 1;
                }
                else
                {
                    /* delete failed */
                    return -1;
                }
            }
            else
            {
                /* delete failed(the communication protocol between server
                   and client has problem) */
                return -1;
            }
        }
    }
}

int main()
{
    cgi_delete(20,"127.0.0.1");
    return 0;
}

