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
#include "../socket_public/message.h"

#define MAXLINE 4096  /* the maxline of the buffer */
#define SERV_PORT 3000  /* port number */
#define RECOVER_MARK_LEN 2 /* the length of the check message mark */

int cgi_recover(int id, char *IP)
{
    int sockfd;
    struct sockaddr_in servaddr;
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
            exit(3);
        }
        else
        {
            /* server response this request */
            if (strncmp(recvline, RECOVER_RESPONSE, RECOVER_MARK_LEN) == 0)
            {

            }
            else
            {
                /* server doesn't response this request */
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
