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
    printf("the time_buffer is %s\n", time_buffer);
}


/* 0 means doesn't exist
   1 means exist */
int is_folder_exist(char *path)
{
    DIR *dp;
    if ((dp = opendir(path)) == NULL)
    {
        return 0;
    }
    closedir(dp);
    return 1;
}
