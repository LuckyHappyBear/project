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
#include "../socket_public/message.h"

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

    strncpy(sendline, CGI_DELETE, DELETE_MARK_LEN);
    memcpy(&sendline[DELETE_MARK_LEN], &id, sizeof(int));
    send(sockfd, sendline, strlen(sendline), 0);

    while (1)
    {
        printf("we reach here or not\n");
        if (recv(sockfd, recvline, MAXLINE, 0) < 0)
        {
            perror("cgi_check:The server terminated prematurely\n");
            exit(3);
        }
        else
        {
            /* server response this request */
            if (strncmp(recvline, DELETE_RESPONSE, DELETE_MARK_LEN) == 0)
            {
            }
            else
            {

            }
        }
    }
}

int main()
{
    cgi_check("IMSIIS0755110000","127.0.0.1");
    return 0;
}

