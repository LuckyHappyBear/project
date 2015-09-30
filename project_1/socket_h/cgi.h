/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   cgi.h

 Description: the declaration of  functions which cgi will call

 Author:      hexiong

 Email:       hexiong@tp-link.net

--------------------------------------

*************************************************************************/

#ifndef _CGI_H
#define _CGI_H

#ifdef __cplusplus
extern "C" {
#endif

int cgi_check(char *IMSI, char *IP);

int cgi_getlist(char *IMSI, char *IP, char *product_id);

int cgi_backup(char *IMSI, char *IP, char *product_id, char *note, char *file_path);

int cgi_recover(int id, char *IP);

int cgi_delete(int id, char *IP);

#ifdef __cplusplus
}
#endif

#endif
