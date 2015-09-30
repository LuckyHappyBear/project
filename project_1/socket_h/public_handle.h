/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   public_handle.h

 Description: we define some function we will used in both server and
              client in there

 Author:      hexiong

 Email:       hexiong@tp-link.net

--------------------------------------

*************************************************************************/

#ifndef _PUBLIC_HANDLE_H
#define _PUBLIC_HANDLE_H
#include "message.h"

struct version_info
{
    int id;
    char imsi[IMSI_LEN + 1];
    char product_id[PRODUCT_ID_LEN + 1];
    char version_no[VERSION_NUM_LEN + 1];
    char note[MAX_NOTE_LEN + 1];
};

#ifdef __cplusplus
extern "C" {
#endif
void get_version_no(char *time_buffer);

int is_folder_exist(char *path);
#ifdef __cplusplus
}
#endif
#endif
