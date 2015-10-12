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

#define FILE_BUFFER_SIZE 1024
#define CGI_TEST 0
#define LOCATION "/home/luckybear/Downloads/backup/"

struct data_transfer
{
    char buffer[FILE_BUFFER_SIZE];                  /* the data buffer */
    int length;                                     /* the length of the data we will transfer */
};

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
void cons_file_name(char *IMSI, char *product_id, char *version_no, char **file_name);

/******************************************************************************
 Function:     add_note_handle
 Description:  we user this function to handle sql can't insert \ and '
 Input:        note: the note which user write
               note_add: the note we have handled that can insert into database
 Output:       NONE
 Return:       NONE
 Others:       NONE
*******************************************************************************/
void add_note_handle(char *note, char **note_add);

/******************************************************************************
 Function:     get_note_handle
 Description:  we user this function to recover note get from database
 Input:        note:the note insert into database
               note_get: the note we get from database
 Output:       NONE
 Return:       NONE
 Others:       NONE
*******************************************************************************/
void get_note_handle(char *note, char** note_get);
#ifdef __cplusplus
}
#endif
#endif
