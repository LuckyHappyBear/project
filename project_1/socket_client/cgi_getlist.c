/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   cgi_getlist.c

 Description: we get backup list information here

 Author:      hexiong

 Email:       hexiong@tp-link.net

--------------------------------------

*************************************************************************/

#include <stdio.h>
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
#define GETLIST_MARK_LEN 2 /* the length of the check message mark */

/******************************************************************************
 Function:     cgi_getlist
 Description:  get the the user's  backup list aimed at a product
 Input:        IMSI:International Mobile Subscriber Identification Number
               IP:the server IP
               product_id: the id of the product
 Output:       NONE
 Return:       the list of the backup aimed at the product
 Others:       NONE
*******************************************************************************/
int cgi_getlist(char *IMSI, char *IP, char *product_id, struct version_info **ver_list)
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
        perror("cgi_getlist:Problem in creating the socket\n");
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
        perror("cgi_getlist:Problem in connecting to the server");
        #endif
        return -1;
    }

    /* message mark field */
    strncpy(sendline, CGI_GETLIST, GETLIST_MARK_LEN);

    /* IMSI field */
    strncpy(&sendline[GETLIST_MARK_LEN], IMSI, IMSI_LEN);

    /* product_id field */
    #if CGI_TEST
    printf("The product_id is %s\n",product_id);
    #endif
    start_pos = GETLIST_MARK_LEN + IMSI_LEN;
    strncpy(&sendline[start_pos], product_id, PRODUCT_ID_LEN);

    #if CGI_TEST
    printf("the sendbuf is %s\n",sendline);
    printf("the send product_id is %s\n", &sendline[start_pos]);
    #endif
    /* send request information to server */
    send(sockfd, sendline, start_pos +  PRODUCT_ID_LEN, 0);

    #if CGI_TEST
    printf("the sendline is %s\n",sendline);
    #endif

    memset(sendline, 0, MAXLINE);
    //printf("the length is %s\n", &sendline[start_pos]);
    while (1)
    {
        if (recv(sockfd, recvline, MAXLINE, 0) > 0)
        {
            if (strncmp(recvline, GETLIST_RESPONSE, GETLIST_MARK_LEN) == 0)
            {
                /* loop variable */
                int i;

                /* the amount of backup version */
                int list_num;

                int length = sizeof(struct version_info);

                #if CGI_TEST
                printf("The length is %d\n",length);
                #endif

                /* get the number of backup version */
                char list[2];
                strncpy(list, &recvline[2], 2);
                list_num = atoi(list);
                (*ver_list) = malloc(list_num * sizeof(struct version_info));
                start_pos = 4;

                #if CGI_TEST
                printf("The list_num is %d\n",list_num);
                #endif

                /* get the list that server return */
                for (i = 0; i < list_num; i ++)
                {
                    memcpy(&(*ver_list)[i], &recvline[start_pos], length);
                    start_pos += length;
                }

                return list_num;
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
            perror("cgi_getlist:The server terminated prematurely\n");
            #endif
            return -1;
        }
    }
}

