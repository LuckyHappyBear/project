/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   database.h

 Description: the declaration of  functions which database.c will call

 Author:      Huang Guozhi

 Email:       huangguozhi@tp-link.net

--------------------------------------

*************************************************************************/

#ifndef _DATABASE_H
#define _DATABASE_H

#include "/usr/local/mysql/include/mysql.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 Function:     mysql_connect_ptr
 Description:  use it to connect to database
 Input:        none
 Output:       whether connect to database successful or not
 Return:       the ptr to a connection to database
 Others:       NONE
*******************************************************************************/
MYSQL *mysql_connect_ptr();

/******************************************************************************
 Function:     getList
 Description:  according the specific IMSI and product_id to get all of the list one user config
 Input:        conn_ptr:the ptr to connect database
               IMSI: IMSI
               product_id: the id of the product
 Output:       NONE
 Return:       the list of the backup aimed at the product
 Others:       NONE
*******************************************************************************/
int getList(MYSQL *conn_ptr, char *IMSI, char *product_id,
            struct version_info **ver_list);

/******************************************************************************
 Function:     add
 Description:  backup a local config to server
 Input:        IMSI:International Mobile Subscriber Identification Number
               conn_ptr:the ptr to connect database
               version_no:the version of this backup
               product_id: the id of the product
               note: the note which user writes
 Output:       NONE
 Return:       1:successfule 0:failed
 Others:       NONE
*******************************************************************************/
int add(MYSQL *conn_ptr,char *IMSI,char *version_no,char *product_id,char *note,char *file_path);

/******************************************************************************
 Function:     query
 Description:  pull a backup from server to local
 Input:        id: the id of this record in database
               conn_ptr:the ptr to connect database
               IMSI: imsi
 Output:       NONE
 Return:       1:recover successfully 0:recover failed
 Others:       NONE
*******************************************************************************/
char *recover(MYSQL *conn_ptr,int id,char *IMSI);

/******************************************************************************
 Function:     delete
 Description:  delete a backup from server
 Input:        id: the id of this record in database
               IMSI:the id of the user
 Output:       NONE
 Return:       1:delete successful 0:delete failed
 Others:       NONE
*******************************************************************************/
int delete(MYSQL *conn_ptr,int id,char *IMSI);


/******************************************************************************
 Function:     count
 Description:  count all the record's amount
 Input:        id: the id of this record in database
               IMSI:the id of the user
 Output:       NONE
 Return:       the number of the space that user has used
 Others:       NONE
*******************************************************************************/
int count(MYSQL *conn_ptr,char *IMSI);


char *IdVsImsi(MYSQL *conn_ptr,int id);

#ifdef __cplusplus
}
#endif

#endif
