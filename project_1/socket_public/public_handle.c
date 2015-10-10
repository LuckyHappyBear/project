/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   public_handle.c

 Description: we define function used by both server and client in this
              file

 Author:      hexiong

 Email:       hexiong@tp-link.net

--------------------------------------

*************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../socket_h/public_handle.h"

/******************************************************************************
 Function:     get_version_no
 Description:  we user this function to give a config version number
 Input:        time_buffer:time_buffer will store the version number
 Output:       NONE
 Return:       NONE
 Others:       NONE
*******************************************************************************/
void get_version_no(char *time_buffer)
{
    char time_tmp[4];
    time_t time_p;
    struct tm *p;
    time(&time_p);
    p = localtime(&time_p);
    memset(time_buffer, 0, 14);
    memset(time_tmp,0, 4);
    /* get year */
    sprintf(time_tmp, "%d", 1900 + p->tm_year);
    strncpy(time_buffer, time_tmp, 4);
    memset(time_tmp, 0, 4);

    sprintf(time_tmp, "%.2d", p->tm_mon + 1);
    strncpy(&time_buffer[4], time_tmp, strlen(time_tmp));
    memset(time_tmp, 0, 4);

    sprintf(time_tmp, "%.2d", p->tm_mday);
    strncpy(&time_buffer[6], time_tmp, strlen(time_tmp));
    memset(time_tmp, 0, 4);

    sprintf(time_tmp, "%.2d", p->tm_hour);
    strncpy(&time_buffer[8], time_tmp, strlen(time_tmp));
    memset(time_tmp, 0, 4);

    sprintf(time_tmp, "%.2d", p->tm_min);
    strncpy(&time_buffer[10], time_tmp, strlen(time_tmp));
    memset(time_tmp, 0, 4);

    sprintf(time_tmp, "%.2d", p->tm_sec);
    strncpy(&time_buffer[12], time_tmp, strlen(time_tmp));
    memset(time_tmp, 0, 4);
    #if CGI_TEST
    printf("the time_buffer is %s\n", time_buffer);
    #endif
}

/******************************************************************************
 Function:     cons_file_name
 Description:  we user this function to give a name to backup file
 Input:        IMSI: International Mobile Subscriber Identification number
               product_id: the id of the product
               version_no: the version of this backup
               file_name: the result we want to get
 Output:       NONE
 Return:       NONE
 Others:       NONE
*******************************************************************************/
void cons_file_name(char *IMSI, char *product_id, char *version_no, char **file_name)
{
    int start_pos = 0;

    strncpy((*file_name), IMSI, strlen(IMSI));
    start_pos += strlen(IMSI);

    //printf("cons_file_name:the imsi is %s\n", IMSI);

    //printf("The filename is %s\n", (*file_name));

    (*file_name)[start_pos] = '_';
    start_pos += 1;
    //printf("The filename is %s\n", (*file_name));

    strncpy(&((*file_name)[start_pos]), product_id, strlen(product_id));
    start_pos += strlen(product_id);
    //printf("The filename is %s\n", (*file_name));

    (*file_name)[start_pos] = '_';
    start_pos += 1;
    //printf("The filename is %s\n", (*file_name));

    strncpy(&((*file_name)[start_pos]), version_no, strlen(version_no));
    start_pos += strlen(version_no);

    strncpy(&((*file_name)[start_pos]), ".tar", strlen(".tar"));
    //printf("The filename is %s\n", (*file_name));
}
