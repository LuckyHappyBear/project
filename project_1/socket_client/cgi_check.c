/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   cgi_check.c

 Description: this file use to complete cgi_check function

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
#include "../socket_h/cgic_client.h"
#include "../socket_h/message.h"
#include "../socket_h/public_handle.h"

#define MAXLINE 4096  /* the maxline of the buffer */
#define SERV_PORT 3000  /* port number */
#define CHECK_MARK_LEN 2 /* the length of the check message mark */
#define USED_FIELD_LEN 2 /* the length remian space field */

/******************************************************************************
 Function:     cgi_check
 Description:  check the space user used in server
 Input:        IMSI:International Mobile Subscriber Identification Number
               IP:the server IP
 Output:       NONE
 Return:       the amount of the backups
 Others:       NONE
*******************************************************************************/
int cgi_check(char *IMSI, char *IP)
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
        #if CGI_TEST
        perror("cgi_check:Problem in creating the socket\n");
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
        perror("cgi_check:Problem in connecting to the server");
        #endif
        return -1;
    }

    /* send check request and IMSI to server */
    strncpy(sendline, CGI_CHECK, CHECK_MARK_LEN);
    strncpy(&sendline[CHECK_MARK_LEN], IMSI, strlen(IMSI));
    send(sockfd, sendline, CHECK_MARK_LEN + strlen(IMSI), 0);
    memset(sendline, 0, MAXLINE);

    while (1)
    {
        if (recv(sockfd, recvline, MAXLINE, 0) < 0)
        {
            #if CGI_TEST
            perror("cgi_check:The server terminated prematurely\n");
            #endif
            return -1;
        }
        else
        {
            /* server response this request */
            if (strncmp(recvline, CHECK_RESPONSE, CHECK_MARK_LEN) == 0)
            {
                /* get the */
                char used_space[USED_FIELD_LEN];
                strncpy(used_space, &recvline[CHECK_MARK_LEN], USED_FIELD_LEN);
                return_code = atoi(used_space);
                #if CGI_TEST
                printf("the recvline is %s\n", recvline);
                #endif
                return return_code;
            }
            else
            {
                /* server doesn't response this request */
                return_code = -1;
                return return_code;
            }
        }
    }
}

