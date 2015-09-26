/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   listen_and_handle.c

 Description: server use this function to listen client's request and
              handle every situation acording to request type

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
#include "../socket_h/message.h"

#define MAXLINE 4096 /* max buffer length */
#define SERV_PORT 3000 /* port number */
#define LISTENQ 20 /* maximum number of client connections */
#define RESPONSE_MARK_LEN 2 /* the response mark's length */
#define IMSI_LEN 15 /* the length of the IMSI*/
#define REMAIN_FIELD_LEN 2 /* the length of the remain field in check respose */
char* itoa(int num, char*str, int radix)
{
    char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum;
    int i = 0,j,k;

    if (radix == 10 && num < 0)
    {
        unum = (unsigned)-num;
        str[i++] = '-';

    }
    else
    {
        unum = (unsigned)num;
    }
    do
    {
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    }while (unum);

    str[i]='\0';

    if (str[0] == '-')
    {
        k = 1;
    }
    else
    {
        k = 0;
    }

    char temp;

    for (j = k; j <= (i - 1) / 2; j++)
    {
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;

    }
    return str;
}


int main()
{
    pid_t childpid;
    socklen_t clilen;
    int listenfd, connfd;
    char sendbuf[MAXLINE], recvbuf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    /* initialize the send and receive buffer */
    /* create a socket */
    if (listenfd = socket(AF_INET, SOCK_STREAM, 0) < 0)
    {
        perror("Problem in creating the socket");
        exit(1);
    }
    /* preparation of the socket address */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    /* bind the socket */
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    /* listen to the socket by creating a connection queue,
       then wait for clients */
    listen(listenfd, LISTENQ);

    printf("Server running...waiting for connections.\n");

    for (;;)
    {
        clilen = sizeof(cliaddr);
        /* accept a connection */
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        printf("Received request...\n");

        if ((childpid = fork()) == 0)
        {
            printf("Child created for dealing with client requests");

            /* close listening socket */
            close(listenfd);

            while (1)
            {
                if (recv(connfd, recvbuf, MAXLINE, 0) < 0)
                {
                    perror("recv error\n");
                    exit(2);
                }
                else
                {
                    if (strncmp(recvbuf,CHECK_RESPONSE,
                                RESPONSE_MARK_LEN) == 0)
                    {
                        int remain = 10;
                        char IMSI[IMSI_LEN];
                        char remain_space[REMAIN_FIELD_LEN];
                        itoa(remain, remain_space, 10);
                        strncpy(IMSI, &recvbuf[RESPONSE_MARK_LEN],
                                IMSI_LEN);
                        printf("The IMSI is %s\n",IMSI);
                        strncpy(sendbuf, CHECK_RESPONSE, RESPONSE_MARK_LEN);
                        strncpy(&sendbuf[RESPONSE_MARK_LEN], remain_space,
                                REMAIN_FIELD_LEN);
                        send(connfd, sendbuf, strlen(sendbuf), 0);
                    }
                }
            }
        }
    }
}
