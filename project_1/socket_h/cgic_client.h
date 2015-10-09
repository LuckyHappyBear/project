/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   cgi.h

 Description: the declaration of  functions which cgi will call

 Author:      hexiong

 Email:       hexiong@tp-link.net

--------------------------------------

*************************************************************************/
#include "public_handle.h"

#ifndef _CGI_H
#define _CGI_H

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 Function:     cgi_check
 Description:  check the space user used in server
 Input:        IMSI:International Mobile Subscriber Identification Number
               IP:the server IP
 Output:       NONE
 Return:       the amount of the backups
 Others:       NONE
*******************************************************************************/
int cgi_check(char *IMSI, char *IP);

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
int cgi_getlist(char *IMSI, char *IP, char *product_id, struct version_info **ver_list);

/******************************************************************************
 Function:     cgi_backup
 Description:  backup a local config to server
 Input:        IMSI:International Mobile Subscriber Identification Number
               IP:the server IP
               product_id: the id of the product
               note: the note user input
               file_path: the path of the config
 Output:       NONE
 Return:       1:successfule -1:failed
 Others:       NONE
*******************************************************************************/
int cgi_backup(char *IMSI, char *IP, char *product_id, char *note, char *file_path);

/******************************************************************************
 Function:     cgi_recover
 Description:  pull a backup from server to local
 Input:        id: the id of this record in database
               IP:the server IP
 Output:       NONE
 Return:       1:recover successfully -1:recover failed
 Others:       NONE
*******************************************************************************/
int cgi_recover(int id, char *IP, char *IMSI);

/******************************************************************************
 Function:     cgi_delete
 Description:  delete a backup from server
 Input:        id: the id of this record in database
               IP: the server IP
               IMSI:International Mobile Subscriber Identification Number
 Output:       NONE
 Return:       1:delete successful -1:delete failed
 Others:       NONE
*******************************************************************************/
int cgi_delete(int id, char *IP, char *IMSI);

#ifdef __cplusplus
}
#endif

#endif
