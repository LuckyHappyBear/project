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
#include "../socket_public/message.h"
#include "../socket_public/public_handle.h"

#define MAXLINE 4096  /* the maxline of the buffer */
#define SERV_PORT 3000  /* port number */
#define GETLIST_MARK_LEN 2 /* the length of the check message mark */
#define FILE_BUFFER_SIZE 4000 /* the size of the file buffer */

int cgi_getlist(char *IMSI, char *IP, char *product_id)
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
        perror("cgi_getlist:Problem in creating the socket\n");
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
        perror("cgi_getlist:Problem in connecting to the server");
        exit(2);
    }

    /* message mark field */
    strncpy(sendline, CGI_GETLIST, GETLIST_MARK_LEN);

    /* IMSI field */
    strncpy(&sendline[GETLIST_MARK_LEN], IMSI, strlen(IMSI));

    /* product_id field */
    start_pos = GETLIST_MARK_LEN + IMSI_LEN;
    strncpy(&sendline[start_pos], product_id, strlen(product_id));

    send(sockfd, sendline, strlen(sendline), 0);
    printf("the sendline is %s\n",sendline);
    memset(sendline, 0, MAXLINE);
    //printf("the length is %s\n", &sendline[start_pos]);
    while (1)
    {
        printf("we reach here or not\n");
        if (recv(sockfd, recvline, MAXLINE, 0) > 0)
        {
            if (strncmp(recvline, GETLIST_RESPONSE, GETLIST_MARK_LEN) == 0)
            {
                /* loop variable */
                int i;

                /* the amount of backup version */
                int list_num;

                int length = sizeof(struct version_info);
                printf("The length is %d\n",length);
                /* get the number of backup version */
                char list[2];
                strncpy(list, &recvline[2], 2);
                list_num = atoi(list);
                struct version_info *ver_list = malloc(list_num * sizeof(struct version_info));
                start_pos = 4;
                printf("The list_num is %d\n",list_num);

                for (i = 0; i < list_num; i ++)
                {
                    memcpy(&ver_list[i], &recvline[start_pos], length);
                    start_pos += length;
                }



                /* we neet to release this space in somewhere,do not forget */
                for(i = 0; i < list_num; i ++)
                {
                    printf("The id is %d\nThe imsi is %s\nThe product_id is %s\nThe version_no is %s\nThe note is %s\n",ver_list[i].id, ver_list[i].imsi,ver_list[i].product_id, ver_list[i].version_no, ver_list[i].note);
                }
                return list_num;
            }
            else
            {
                /* server doesn't response this request */
                return_code = -1;
                memset(&sendline, 0, MAXLINE);
                memset(&recvline, 0, MAXLINE);
                return return_code;
            }
        }
        else
        {
            perror("cgi_getlist:The server terminated prematurely\n");
            exit(3);
        }
    }
}

int main()
{
    cgi_getlist("123456789871111","127.0.0.1","73000001");
    return 0;
}
