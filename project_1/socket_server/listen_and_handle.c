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
#include <unistd.h>
#include <string.h>
#include "../socket_public/message.h"
#include "../socket_public/public_handle.h"

#define MAXLINE 4096 /* max buffer length */
#define SERV_PORT 3000 /* port number */
#define LISTENQ 20 /* maximum number of client connections */
#define RESPONSE_MARK_LEN 2 /* the response mark's length */
#define REMAIN_FIELD_LEN 2 /* the length of the remain field in check respose */
#define FILE_BUFFER_LEN 4000 /* the length of the file buffer */

int main()
{
    socklen_t clilen;
    int listenfd, connfd;
    char file_path[512];
    char IMSI[IMSI_LEN + 1];
    char file_buffer[FILE_BUFFER_LEN];
    char sendbuf[MAXLINE], recvbuf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    /* initialize the send and receive buffer */
    memset(sendbuf, 0, MAXLINE);
    memset(recvbuf, 0, MAXLINE);

    /* create a socket */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
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

    while (1)
    {
        clilen = sizeof(cliaddr);
        /* accept a connection */
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        printf("Received request...\n");

        while (recv(connfd, recvbuf, MAXLINE, 0) > 0)
        {
            if (strncmp(recvbuf, CHECK_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                int remain = 10;
                char remain_space[REMAIN_FIELD_LEN];
                sprintf(remain_space, "%d",remain);
                strncpy(IMSI, &recvbuf[RESPONSE_MARK_LEN],
                        IMSI_LEN);
                IMSI[IMSI_LEN] = '\0';
                printf("The IMSI is %s\n",IMSI);
                strncpy(sendbuf, CHECK_RESPONSE, RESPONSE_MARK_LEN);
                strncpy(&sendbuf[RESPONSE_MARK_LEN], remain_space,
                        REMAIN_FIELD_LEN);
                send(connfd, sendbuf, strlen(sendbuf), 0);
                memset(sendbuf, 0, MAXLINE);
                memset(recvbuf, 0, MAXLINE);
            }
            else if (strncmp(recvbuf, BACKUP_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                /* receive the backup request from client */
                int start_pos = 3;
                if (recvbuf[2] == '0')
                {
                    struct version_info* ver = malloc(sizeof(*ver));
                    if(NULL == ver)
                    {
                        perror("space allocate failed\n");
                    }
                    //printf("the size is sizeof(*ver):%d\n",sizeof(*ver));
                    memset(ver, 0, sizeof(struct version_info));

                    /* assign every struct field */
                    /* IMSI field */
                    strncpy(ver->imsi, &recvbuf[start_pos],IMSI_LEN);
                    ver->imsi[IMSI_LEN] = '\0';
                    //printf("the imsi is %s,the length is %d\n",ver->imsi,strlen(ver->imsi));
                    /* product_id field */
                    start_pos += IMSI_LEN;
                    strncpy(ver->product_id, &recvbuf[start_pos],
                            PRODUCT_ID_LEN);
                    ver->product_id[PRODUCT_ID_LEN] = '\0';
                    //printf("the product id is %s,the length is %d\n",ver->product_id,strlen(ver->product_id));

                    /* version_no field */
                    start_pos += PRODUCT_ID_LEN;
                    strncpy(ver->version_no, &recvbuf[start_pos],
                           VERSION_NUM_LEN);
                    ver->version_no[VERSION_NUM_LEN] = '\0';

                    /* note field */
                    start_pos += VERSION_NUM_LEN;
                    strncpy(ver->note, &recvbuf[start_pos], MAX_NOTE_LEN);
                    ver->note[strlen(ver->note)] = '\0';
                    printf("The imsi is %s\nThe product_id is %s\nThe version_no is %s\nThe note is %s\n",ver->imsi,ver->product_id,ver->version_no,ver->note);
                    strncpy(sendbuf,BACKUP_RESPONSE,RESPONSE_MARK_LEN);
                    send(connfd, sendbuf, strlen(sendbuf), 0);
                    memset(sendbuf, 0, MAXLINE);
                }
                else if(recvbuf[2] == '1')
                {
                    printf("we reach here to save file\n");
                    memset(file_buffer, 0, FILE_BUFFER_LEN);

                    strncpy(file_buffer, &recvbuf[3], strlen(&recvbuf[3]));

                    printf("the file buffer is %s\n", file_buffer);
                    FILE *fp = fopen("/home/luckybear/Documents/test", "w");
                    if(NULL == fp)
                    {
                        printf("Can Not Open To Write\n");
                        exit(1);
                    }
                    fputs(file_buffer, fp);
                    fclose(fp);
                    memset(file_buffer, 0, FILE_BUFFER_LEN);
                }
            }
            else if (strncmp(recvbuf, GETLIST_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                int length = sizeof(struct version_info);
                char product_id[PRODUCT_ID_LEN + 1];
                printf("the recvbuf is %s\n",recvbuf);
                memset(IMSI, 0, strlen(IMSI));
                strncpy(IMSI, &recvbuf[2], IMSI_LEN);
                IMSI[strlen(IMSI)] = '\0';
                strncpy(product_id, &recvbuf[2 + IMSI_LEN], PRODUCT_ID_LEN);
                product_id[PRODUCT_ID_LEN] = '\0';
                printf("The IMSI is %s\nThe product_id is %s\n", IMSI, product_id);
                struct version_info *ver_list = malloc(3 * sizeof(struct version_info));
                ver_list[0].id = 1;
                strcpy(ver_list[0].imsi, "123456789871111");
                strcpy(ver_list[0].product_id, "73000001");
                strcpy(ver_list[0].version_no, "20150928000000");
                strcpy(ver_list[0].note,"I love c++");

                ver_list[1].id = 2;
                strcpy(ver_list[1].imsi, "123456789872222");
                strcpy(ver_list[1].product_id, "73000002");
                strcpy(ver_list[1].version_no, "20150928000011");
                strcpy(ver_list[1].note,"I love c++ more than c");

                ver_list[2].id = 3;
                strcpy(ver_list[2].imsi, "123456789873333");
                strcpy(ver_list[2].product_id, "73000003");
                strcpy(ver_list[2].version_no, "20150928000022");
                strcpy(ver_list[2].note,"I love c++ more than cccc");

                int i;
                for(i = 0; i < 3; i ++)
                {
                    printf("The id is %d\nThe imsi is %s\nThe product_id is %s\nThe version_no is %s\nThe note is %s\n",ver_list[i].id, ver_list[i].imsi,ver_list[i].product_id, ver_list[i].version_no, ver_list[i].note);
                }

                memset(sendbuf, 0, MAXLINE);
                strncpy(sendbuf, GETLIST_RESPONSE, RESPONSE_MARK_LEN);
                sprintf(&sendbuf[2], "%.2d", 3);
                memcpy(&sendbuf[4], &ver_list[0], length);
                printf("The length is %d\n", length);
                struct version_info *vert;
                vert = (struct version_info*)&sendbuf[4];
                printf("The id is %d\nThe imsi is %s\nThe product_id is %s\nThe version_no is %s\nThe note is %s\n",vert->id, vert->imsi,vert->product_id, vert->version_no, vert->note);
                memcpy(&sendbuf[4 + length], &ver_list[1], length);
                memcpy(&sendbuf[4 + 2 * length], &ver_list[2], length);
                printf("The sendbuf is %s\n",sendbuf);
                send(connfd, sendbuf, 4+length*3, 0);
            }
            else if (strncmp(recvbuf, DELETE_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                int id;
                memcpy(&id, &recvbuf[2], sizeof(int));
                printf("the id is %d\n",id);
            }
        }
    }
}
