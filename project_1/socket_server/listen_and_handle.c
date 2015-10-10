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
#include "../socket_h/database.h"
#include "../socket_h/message.h"
#include "../socket_h/public_handle.h"
#include "/usr/local/mysql/include/mysql.h"

#define MAXLINE 4096 /* max buffer length */
#define SERV_PORT 3000 /* port number */
#define LISTENQ 20 /* maximum number of client connections */
#define RESPONSE_MARK_LEN 2 /* the response mark's length */
#define USED_FIELD_LEN 2 /* the length of the remain field in check respose */
#define FILE_BUFFER_LEN 4000 /* the length of the file buffer */

int main()
{
    socklen_t clilen;
    int listenfd, connfd;
    char IMSI[IMSI_LEN + 1];
    char sendbuf[MAXLINE], recvbuf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    /* initialize the send and receive buffer */
    memset(sendbuf, 0, MAXLINE);
    memset(recvbuf, 0, MAXLINE);

    MYSQL *conn_ptr;
    conn_ptr = mysql_init(NULL);

    if((conn_ptr = mysql_connect_ptr()) == NULL)
    {
        perror("Connect to Database failed");
    }

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

        /* file_name we will use */
        char *file_name = malloc(256 * sizeof(char));
        memset(file_name, 0, 256);
        if (NULL == file_name)
        {
            perror("spcace allocate failed\n");
        }

        /* the struct store version information */
        struct version_info* ver = malloc(sizeof(*ver));
        memset(ver, 0, sizeof(struct version_info));
        if (NULL == ver)
        {
            perror("space allocate failed\n");
        }

        printf("Received request...\n");

        while (recv(connfd, recvbuf, MAXLINE, 0) > 0)
        {
            printf("The request is %s\n", recvbuf);
            /* receive check message */
            if (strncmp(recvbuf, CHECK_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                printf("************check function*************\n");
                /* get the IMSI */
                strncpy(IMSI, &recvbuf[RESPONSE_MARK_LEN],
                        IMSI_LEN);
                IMSI[IMSI_LEN] = '\0';
                printf("The IMSI is %s\n",IMSI);

                /* get the used space */
                int used = count(conn_ptr, IMSI);
                printf("The space this user has used is %d", used);
                char used_space[USED_FIELD_LEN];
                sprintf(used_space, "%d", used);

                /* send check request to server */
                strncpy(sendbuf, CHECK_RESPONSE, RESPONSE_MARK_LEN);
                strncpy(&sendbuf[RESPONSE_MARK_LEN], used_space,
                        USED_FIELD_LEN);
                send(connfd, sendbuf, strlen(sendbuf), 0);

                /* clear the array */
                memset(sendbuf, 0, MAXLINE);
                memset(recvbuf, 0, MAXLINE);
                memset(IMSI, 0, IMSI_LEN);
                break;
            }
            /* receive backup message */
            else if (strncmp(recvbuf, BACKUP_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                printf("************backup function*************\n");
                //printf("we reach here or not(backup request.....)\n");
                char file_path[512];
                /* receive the backup request from client */
                if (recvbuf[2] == '0')
                {
                    int start_pos = 3;

                    /* assign every struct field */
                    /* IMSI field */
                    strncpy(ver->imsi, &recvbuf[start_pos],IMSI_LEN);

                    //ver->imsi[IMSI_LEN] = '\0';
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

                    /* use this informaion to construct file's name */
                    cons_file_name(ver->imsi, ver->product_id, ver->version_no, &file_name);

                    /* send message to client that server receive this request */
                    strncpy(sendbuf,BACKUP_RESPONSE,RESPONSE_MARK_LEN);
                    send(connfd, sendbuf, strlen(sendbuf), 0);
                    memset(sendbuf, 0, MAXLINE);
                    memset(recvbuf, 0, MAXLINE);
                }
                else if(recvbuf[2] == '1')
                {
                    printf("The recvbuf is %s\n", recvbuf);
                    //printf("the first recvbuf length is %d\n", strlen(recvbuf));

                    /* get the storage location */
                    memset(file_path, 0, 512);
                    strncat(file_path, LOCATION, strlen(LOCATION));
                    strncat(file_path, file_name, strlen(file_name));

                    printf("The file path is %s\n", file_path);

                    FILE *fp = fopen(file_path, "ab");
                    struct data_transfer *data = malloc(sizeof(*data));
                    int length = 0;
                    memset(recvbuf, 0, MAXLINE);
                    memset(sendbuf, 0, MAXLINE);

                    /* tell client that you can send data */
                    sendbuf[0] = '1';
                    sendbuf[1] = '\0';
                    send(connfd, sendbuf, strlen(sendbuf), 0);
                    memset(sendbuf, 0, MAXLINE);
                    memset(recvbuf, 0, MAXLINE);

                    /* begin to receive and write the data */
                    while ((length = recv(connfd, recvbuf, MAXLINE, 0)) > 0)
                    {
                        /* receive data */
                        if (recvbuf[0] == '1')
                        {
                            //printf("**************************************\n");
                            memcpy(data, &recvbuf[1], sizeof(*data));
                            fwrite(data->buffer, 1, data->length, fp);
                            //printf("The buffer we will write is %s\n", data->buffer);
                            //printf("The length we will write is %d\n", data->length);
                            memset(recvbuf, 0, MAXLINE);
                            memset(data->buffer, 0, FILE_BUFFER_SIZE);
                            memset(sendbuf, 0, MAXLINE);
                            printf("we reach here to send something\n");

                            /* send message to tell client to send next data buffer */
                            sendbuf[0] = '1';
                            sendbuf[1] = '\0';
                            printf("the sendline is %s\n", sendbuf);
                            send(connfd, sendbuf, 2, 0);
                        }
                        else
                        {
                            break;
                        }
                    }
                    if(length < 0)
                    {
                        perror("Transfer error");
                        break;
                    }
                    else
                    {
                        /* receive successful and write completely */
                        printf("we reach here?\n");
                        fclose(fp);
                        printf("The imsi is %s\nThe product_id is %s\nThe version_no is %s\nThe note is %s\n",ver->imsi,ver->product_id,ver->version_no,ver->note);
                        printf("The file_path is %s\n", file_path);

                        /* add ro database successful */
                        if (add(conn_ptr, ver->imsi, ver->version_no,
                                ver->product_id, ver->note, file_path))
                        {
                            /* send successful message to client */
                            memset(sendbuf, 0, MAXLINE);
                            sendbuf[0] = 'A';
                            sendbuf[1] = '\0';
                            send(connfd, sendbuf, 2, 0);
                        }
                        else
                        {
                            /* send failed message to client */
                            memset(sendbuf, 0, MAXLINE);
                            sendbuf[0] = 'B';
                            sendbuf[1] = '\0';
                            send(connfd, sendbuf, 2, 0);
                        }
                        memset(recvbuf, 0, MAXLINE);
                        memset(sendbuf, 0, MAXLINE);
                        free(ver);
                        free(file_name);
                        free(data);
                        break;
                    }
                    break;
                }
            }
            /* receive getlist message */
            else if (strncmp(recvbuf, GETLIST_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                printf("************getlist function*************\n");
                int list_num;
                int length = sizeof(struct version_info);
                char product_id[PRODUCT_ID_LEN + 1];
                printf("the recvbuf is %s\n",recvbuf);
                memset(IMSI, 0, strlen(IMSI));
                strncpy(IMSI, &recvbuf[2], IMSI_LEN);
                IMSI[strlen(IMSI)] = '\0';
                printf("the protadsfasdf is %s\n", &recvbuf[17]);
                strncpy(product_id, &recvbuf[2 + IMSI_LEN], PRODUCT_ID_LEN);
                product_id[PRODUCT_ID_LEN] = '\0';
                printf("The IMSI is %s\nThe product_id is %s\n", IMSI, product_id);

                struct version_info *ver_list;
                list_num = getList(conn_ptr, IMSI, product_id, &ver_list);
                printf("the number of backup is %d\n", list_num);

                int i;
                for(i = 0; i < list_num; i ++)
                {
                    printf("The id is %d\nThe imsi is %s\nThe product_id is %s\nThe version_no is %s\nThe note is %s\n",ver_list[i].id, ver_list[i].imsi,ver_list[i].product_id, ver_list[i].version_no, ver_list[i].note);
                }

                memset(sendbuf, 0, MAXLINE);
                strncpy(sendbuf, GETLIST_RESPONSE, RESPONSE_MARK_LEN);
                sprintf(&sendbuf[2], "%.2d", list_num);
                for (i = 0; i < list_num; i ++)
                {
                    memcpy(&sendbuf[4 + length * i], &ver_list[i], length);
                }
                send(connfd, sendbuf, 4 + length * list_num, 0);
                memset(sendbuf, 0, MAXLINE);
                memset(IMSI, 0, IMSI_LEN);
                break;
            }
            /* receive delete message */
            else if (strncmp(recvbuf, DELETE_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                printf("************delete function*************\n");
                int id;
                char *id_imsi;
                int  del_result;
                char file_path[512];
                memset(file_path, 0, 512);
                memset(IMSI, 0, IMSI_LEN + 1);
                memcpy(&id, &recvbuf[2 + IMSI_LEN], sizeof(int));
                strncpy(IMSI, &recvbuf[2], IMSI_LEN);
                printf("the imsi is %s\n", IMSI);
                printf("the id is %d\n",id);


                /* we do delete operations here */
                printf("***************************\n");
                strcpy(file_path, recover(conn_ptr, id, IMSI));
                remove(file_path);
                del_result = delete(conn_ptr, id, IMSI);
                printf("The file_path is %s\n",file_path);
                printf("***************************\n");
                if (del_result)
                {
                    memset(sendbuf, 0, MAXLINE);
                    strncpy(sendbuf, DELETE_RESPONSE, RESPONSE_MARK_LEN);
                    /* means delete successful */
                    sendbuf[2] = 'A';
                    printf("the sendbuf is %s\n", sendbuf);
                    send(connfd, sendbuf, strlen(sendbuf), 0);
                }
                else
                {
                    memset(sendbuf, 0, MAXLINE);
                    strncpy(sendbuf, DELETE_RESPONSE, RESPONSE_MARK_LEN);
                    /* means delete successful */
                    sendbuf[2] = 'B';
                    printf("the sendbuf is %s\n", sendbuf);
                    send(connfd, sendbuf, strlen(sendbuf), 0);
                }
                memset(sendbuf, 0, MAXLINE);
                memset(IMSI, 0, IMSI_LEN);
                break;
            }
            /* receive recover message */
            else if (strncmp(recvbuf, RECOVER_RESPONSE, RESPONSE_MARK_LEN) == 0)
            {
                printf("************recover function*************\n");
                int id;
                /* get the IMSI */
                strncpy(IMSI, &recvbuf[2], IMSI_LEN);
                /* get the id in database */
                memcpy(&id, &recvbuf[2 + IMSI_LEN], sizeof(int));
                char file_path[512];

                memset(file_path, 0, 512);
                memset(sendbuf, 0, MAXLINE);
                strcpy(file_path, recover(conn_ptr, id, IMSI));
                printf("The file_path is %s\n", file_path);

                FILE *fp = fopen(file_path,"rb");
                struct data_transfer *data = malloc(sizeof(*data));
                if(NULL == fp)
                {
                    printf("File Not Found\n");
                    strncpy(sendbuf, RECOVER_RESPONSE, RESPONSE_MARK_LEN);
                    sendbuf[2] = 'B';
                    send(connfd, sendbuf, strlen(sendbuf), 0);
                    memset(sendbuf, 0, MAXLINE);
                    break;
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

                    strncpy(sendbuf, RECOVER_RESPONSE, RESPONSE_MARK_LEN);
                    sendbuf[2] = 'A';
                    send(connfd, sendbuf, strlen(sendbuf), 0);
                    memset(sendbuf, 0, MAXLINE);

                    while (recv(connfd, recvbuf, MAXLINE, 0) > 0)
                    {
                        if(recvbuf[0] == '1')
                        {
                            break;
                        }
                    }

                    while (1)
                    {
                        if ((left_len <= 0) || (read_len >= total_bytes))
                        {
                            fclose(fp);
                            memset(sendbuf, 0, MAXLINE);
                            sendbuf[0] = '0';
                            sendbuf[1] = '\0';
                            send(connfd, sendbuf, strlen(sendbuf), 0);
                            memset(sendbuf, 0, MAXLINE);
                            printf("we finished here\n");
                            free(data);
                            break;
                        }

                        if (left_len >= FILE_BUFFER_SIZE)
                        {
                            memset(data->buffer, 0, FILE_BUFFER_SIZE);
                            length = fread(data->buffer, 1, FILE_BUFFER_SIZE,
                                          fp);
                            data->length = length;
                            printf("The length is %d\n", length);
                            read_len += length;
                            printf("The read_len is %d\n", read_len);
                        }
                        else
                        {
                            memset(data->buffer, 0, FILE_BUFFER_SIZE);
                            length = fread(data->buffer, 1, left_len, fp);
                            printf("The length is %d\n" , length);
                            data->length = length;
                            read_len += length;
                            printf("The read_len is %d\n", read_len);
                        }

                        left_len = total_bytes - read_len;

                        sendbuf[0] = '1';
                        memcpy(&sendbuf[1], data, sizeof(*data));
                        /*printf("The length is %d\n",strlen(sendbuf));
                        printf("The sendbuffer is %s\n", sendbuf);*/
                        if (send(connfd, sendbuf, sizeof(*data) + 1, 0) < 0)
                        {
                            perror("Send File Failed");
                            break;
                        }
                        else
                        {
                            printf("we reach here to wait for response\n");
                            memset(recvbuf, 0, MAXLINE);
                            while (recv(connfd, recvbuf, MAXLINE, 0) > 0)
                            {
                                printf("did we reveice the response\n");
                                printf("The recvbuf content is %s\n",recvbuf);
                                if(recvbuf[0] == '1')
                                {
                                    printf("we receive client's response\n");
                                    break;
                                }
                            }
                        }
                        printf("we continue send here\n");
                        memset(sendbuf, 0, MAXLINE);
                    }
                    break;
                }
            }
        }
    }
}
