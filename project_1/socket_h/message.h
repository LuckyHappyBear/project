/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   message.h

 Description: this file define the message type we will use

 Author:      hexiong

 History:     hexiong create the file and complete the function

--------------------------------------

*************************************************************************/

#ifndef _MESSAGE_H
#define _MESSAGE_H

/* client message */
#define CGI_CHECK   "00"
#define CGI_BACKUP  "01"
#define CGI_GETLISE "02"
#define CGI_DELETE  "03"
#define CGI_RECOVER "04"

/* server message */
#define CHECK_RESPONSE    "00"
#define BACKUP_RESPONSE   "01"
#define GETLIST_RESPONSE  "02"
#define DELETE_RESPONSE   "03"
#define RECOVER_RESPONSE  "04"

#endif