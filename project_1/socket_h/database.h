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
struct version_info *getList ( MYSQL *conn_ptr,char *IMSI,char *product_id );

/******************************************************************************
 Function:     add
 Description:  backup a local config to server
 Input:        IMSI:International Mobile Subscriber Identification Number
               IP:the server IP
               product_id: the id of the product
               note: the note user input
               file_path: the path of the config
 Output:       NONE
 Return:       1:successfule 0:failed
 Others:       NONE
*******************************************************************************/
int add (MYSQL *conn_ptr,char *IMSI,char *version_no,char *product_id,char *note,char *file_path);

/******************************************************************************
 Function:     query
 Description:  pull a backup from server to local
 Input:        id: the id of this record in database
 Output:       NONE
 Return:       1:recover successfully 0:recover failed
 Others:       NONE
*******************************************************************************/
char *query(MYSQL *conn_ptr,int id,char *IMSI);

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
               product_id:the id of product
 Output:       NONE
 Return:       1:delete successful 0:delete failed
 Others:       NONE
*******************************************************************************/
int count(MYSQL *conn_ptr,char *IMSI,char *product_id);

#ifdef __cplusplus
}
#endif

#endif
