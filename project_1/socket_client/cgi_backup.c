/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   cgi_backup.c

 Description: this file use to complete cgi_backup function

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
#include "../socket_public/message.h"
#include "../socket_public/public_handle.h"

#define MAXLINE 4096  /* the maxline of the buffer */
#define SERV_PORT 3000  /* port number */
#define BACKUP_MARK_LEN 2 /* the length of the check message mark */
#define FILE_BUFFER_SIZE 4000 /* the size of the file buffer */

int cgi_backup(char *IMSI, char *IP, char *product_id, char *note, char *file_path)
{
    int sockfd;
    int start_pos;          /* count every field's start position */
    int return_code;
    int file_len;
    char file_size[5];
    char version_no[14];
    char file_buffer[4000];
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    /* initialize the send buffer and receive buffer */
    memset(&sendline, 0, MAXLINE);
    memset(&sendline, 0, MAXLINE);

    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0)
    {
        perror("cgi_backup:Problem in creating the socket\n");
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
        perror("cgi_backup:Problem in connecting to the server");
        exit(2);
    }

    /*FILE *fp = fopen(file_path, "r");
    fseek(fp, 0L, SEEK_END);
    file_len = ftell(fp);
    fclose(fp);
    sprintf(file_size, "%d", file_len);
    printf("the file size is %s\n",file_size);
    printf("the file length is %d\n",file_len);*/
    strncpy(sendline, CGI_BACKUP, BACKUP_MARK_LEN);
    /* the third bit used to ditinguish data package or information package */
    sendline[2] = '0';

    /* IMSI field */
    strncpy(&sendline[BACKUP_MARK_LEN + 1], IMSI, strlen(IMSI));

    /* product_id field */
    start_pos = BACKUP_MARK_LEN + 1 + IMSI_LEN;
    strncpy(&sendline[start_pos], product_id, strlen(product_id));

    /* version_no field */
    start_pos += PRODUCT_ID_LEN;
    get_version_no(version_no);
    strncpy(&sendline[start_pos], version_no, VERSION_NUM_LEN);

    /* note field */
    start_pos += VERSION_NUM_LEN;
    strncpy(&sendline[start_pos], note, strlen(note));

    /* field length field */
    /*start_pos += MAX_NOTE_LEN;
    strncpy(&sendline[start_pos], file_size, strlen(file_size));*/
    send(sockfd, sendline, strlen(sendline), 0);
    printf("the sendline is %s\n",sendline);
    memset(sendline, 0, MAXLINE);
    //printf("the length is %s\n", &sendline[start_pos]);
    while (1)
    {
        printf("we reach here or not\n");
        if (recv(sockfd, recvline, MAXLINE, 0) > 0)
        {
            if (strncmp(recvline, BACKUP_RESPONSE, BACKUP_MARK_LEN) == 0)
            {
                FILE *fp = fopen(file_path, "r");
                if (NULL == fp)
                {
                    printf("File is not found\n");
                }
                else
                {
                    memset(file_buffer, 0, FILE_BUFFER_SIZE);
                    int length = 0;
                    while ((length = fread(file_buffer, sizeof(char),
                                           FILE_BUFFER_SIZE, fp)) > 0)
                    {
                        strncpy(sendline, "011", 3);
                        strncpy(&sendline[3], file_buffer,
                                strlen(file_buffer));
                        printf("the content is %s\n",sendline);
                        if (send(sockfd, sendline, strlen(sendline), 0) < 0)
                        {
                            printf("send file failed\n");
                            break;
                        }
                        memset(file_buffer, 0, FILE_BUFFER_SIZE);
                        memset(sendline, 0, MAXLINE);
                    }
                    fclose(fp);
                    printf("transfer successful\n");
                }
                return return_code;
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
            perror("cgi_backup:The server terminated prematurely\n");
            exit(3);
        }
    }
}

int main()
{
    cgi_backup("123456789871111","127.0.0.1","73000001","the first backup","/home/luckybear/test/testTime.c");
    return 0;
}
