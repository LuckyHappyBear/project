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
#define CGI_CHECK   "00"         /* check message */
#define CGI_BACKUP  "01"         /* backup message */
#define CGI_GETLIST "02"         /* getlist message */
#define CGI_DELETE  "03"         /* delete message */
#define CGI_RECOVER "04"         /* recover message */

/* server message */
#define CHECK_RESPONSE    "00"   /* check response */
#define BACKUP_RESPONSE   "01"   /* backup response */
#define GETLIST_RESPONSE  "02"   /* getlist response */
#define DELETE_RESPONSE   "03"   /* delete response */
#define RECOVER_RESPONSE  "04"   /* recover response */

/* field length */
#define IMSI_LEN 15              /* the length of IMSI */
#define PRODUCT_ID_LEN 8         /* the length of the product id*/
#define VERSION_NUM_LEN 14       /* the length of the version number */
#define MAX_NOTE_LEN 60          /* the max length of the note user can input */

#endif
