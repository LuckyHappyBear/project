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
    int id;                                         /* the id of the backup information in database */
    char imsi[IMSI_LEN + 1];                        /* International Mobile Subscriber Identification Number */
    char product_id[PRODUCT_ID_LEN + 1];            /* the id of the product */
    char version_no[VERSION_NUM_LEN + 1];           /* the version number of this backup */
    char note[MAX_NOTE_LEN + 1];                    /* the note user input */
};

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 Function:     get_version_no
 Description:  we user this function to give a config version number
 Input:        time_buffer:time_buffer will store the version number
 Output:       NONE
 Return:       NONE
 Others:       NONE
*******************************************************************************/
void get_version_no(char *time_buffer);

int is_folder_exist(char *path);
#ifdef __cplusplus
}
#endif
#endif
