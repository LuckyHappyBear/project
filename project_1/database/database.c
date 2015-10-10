/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   database.c

 Description: this file use to complete cgi_backup function

 Author:      Huang Guozhi

 Email:       huangguozhi@tp-link.net

 --------------------------------------

 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "../socket_h/message.h"
#include "../socket_h/public_handle.h"
#include "../socket_h/database.h"
#define EXIT_FAILURE 0
#define EXIT_SUCCESS 1

#define SQL_MAX_LENGTH 1000

#define IP "localhost"
#define DBUSER "root"
#define DBPASSWORD "luckyBear"
#define DB "mifi"




/*connect to database*/
MYSQL *mysql_connect_ptr()
{
    MYSQL *conn_ptr;
  	conn_ptr = mysql_init(NULL);
    if (!conn_ptr)
    {
        printf("mysql_init failed\n");
        return NULL;
    }

    conn_ptr = mysql_real_connect(conn_ptr,IP,DBUSER,DBPASSWORD,DB,0,NULL,0);

    if (conn_ptr)
    {
        return conn_ptr;
    }
    else
    {
        return NULL;
    }
}



int getList (MYSQL *conn_ptr,char *IMSI,char *product_id, struct version_info **ver_list)
{
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    MYSQL_FIELD *fd;
	int res ,i ,j;
	int count = 0;
	int mark;
	char sql[SQL_MAX_LENGTH] = "select id,imsi,version_no,product_id,note from record";
	strcat(sql," where ");
	strcat(sql,"imsi");
	strcat(sql," = '");
	strcat(sql,IMSI);
	strcat(sql,"'");
	strcat(sql," and ");
	strcat(sql,"product_id");
	strcat(sql," = '");
	strcat(sql,product_id);
	strcat(sql,"'");

    res = mysql_query(conn_ptr,sql); //查询语句
    if (res)
    {
        printf("SELECT error:%s\n",mysql_error(conn_ptr));
    }
    else
    {
        res_ptr = mysql_store_result(conn_ptr);             //取出结果集
        if(res_ptr)
        {
            j = mysql_num_fields(res_ptr);  //j is the field in a table
            (*ver_list) = malloc((unsigned long)mysql_num_rows(res_ptr) * sizeof(struct version_info));

            while((sqlrow = mysql_fetch_row(res_ptr)))  {   //依次取出记录
           		mark = count++;
            	(*ver_list)[mark].id = atoi(sqlrow[0]);
            	strcpy((*ver_list)[mark].imsi,sqlrow[1]);
            	strcpy((*ver_list)[mark].version_no,sqlrow[2]);
            	strcpy((*ver_list)[mark].product_id,sqlrow[3]);
            	strcpy((*ver_list)[mark].note,sqlrow[4]);
            }

            if (mysql_errno(conn_ptr))
            {
                fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));
            }
        }
        mysql_free_result(res_ptr);
    }
    return count;
}

int add (MYSQL *conn_ptr,char *IMSI,char *version_no,char *product_id,char *note,char *file_path)
{
	int res;
	char sql[SQL_MAX_LENGTH] = "insert into record(imsi,version_no,product_id,note,file_path) values('";//null,'Ann',5)";
	strcat(sql,IMSI);
	strcat(sql,"','");
	strcat(sql,version_no);
	strcat(sql,"','");
	strcat(sql,product_id);
	strcat(sql,"','");
	strcat(sql,note);
	strcat(sql,"','");
	strcat(sql,file_path);
	strcat(sql,"')");
	printf("\n%s\n",sql);

    res = mysql_query(conn_ptr,sql);   //可以把insert语句替换成delete或者update语句，都一样的

    if (!res) {     //输出受影响的行数
        printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
        return 1;
    }  else {       //打印出错误代码及详细信息
        fprintf(stderr, "Insert error %d: %sn",mysql_errno(conn_ptr),mysql_error(conn_ptr));
        return 0;
    }
}

char *recover(MYSQL *conn_ptr,int id,char *IMSI)
{
	char *s = malloc(10 * sizeof(char));
    memset(s, 0, 10);
    sprintf(s,"%d",id);
	char *file_path;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    MYSQL_FIELD *fd;
    int res, i, j;

    char sql[SQL_MAX_LENGTH] = "select id,file_path from record ";
	strcat(sql," where ");
	strcat(sql,"imsi");
	strcat(sql," = '");
	strcat(sql,IMSI);
	strcat(sql,"'");
	strcat(sql," and ");
	strcat(sql,"id");
	strcat(sql," = '");
	strcat(sql,s);
	strcat(sql,"'");

    res = mysql_query(conn_ptr,sql);
    if (res)
    {
        printf("SELECT error:%s\n",mysql_error(conn_ptr));
        return NULL;
    }
    else
    {
        res_ptr = mysql_store_result(conn_ptr);

        if(res_ptr)
        {
            printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));
            j = mysql_num_fields(res_ptr);

            if((unsigned long)mysql_num_rows(res_ptr)==0){
                return "0";
            }else{
                file_path = mysql_fetch_row(res_ptr)[1];
                if (mysql_errno(conn_ptr)) {
                    fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));
                }
            }
        }else{
            return "0";
        }
        mysql_free_result(res_ptr);
    }
    free(s);
    return file_path;
}

char *IdVsImsi(MYSQL *conn_ptr,int id)
{

    char *s;
    sprintf(s,"%d",id);
    char *file_path;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    MYSQL_FIELD *fd;
    int res, i, j;

    char sql[SQL_MAX_LENGTH] = "select id,imsi from record ";
    strcat(sql," where ");
    strcat(sql,"id");
    strcat(sql," = '");
    strcat(sql,s);
    strcat(sql,"'");

    res = mysql_query(conn_ptr,sql);
    if (res)
    {
        printf("SELECT error:%s\n",mysql_error(conn_ptr));
        return "0";
    }
    else
    {
        res_ptr = mysql_store_result(conn_ptr);

        if(res_ptr)
        {
            printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));
            j = mysql_num_fields(res_ptr);

            if((unsigned long)mysql_num_rows(res_ptr)==0){
                return "0";
            }else{
                file_path = mysql_fetch_row(res_ptr)[1];
                if (mysql_errno(conn_ptr)) {
                    fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));
                }
            }
        }else{
            return "0";
        }
        mysql_free_result(res_ptr);
    }
    return file_path;
}


int count(MYSQL *conn_ptr,char *IMSI)
{

	MYSQL_RES *res_ptr;
	int res;
	int count = 0;
	char sql[SQL_MAX_LENGTH] = "select id,imsi,version_no,product_id,note from record";
	strcat(sql," where ");
	strcat(sql,"imsi");
	strcat(sql," = '");
	strcat(sql,IMSI);
	strcat(sql,"'");

    res = mysql_query(conn_ptr,sql); //查询语句
    if (res)
    {
        printf("SELECT error:%s\n",mysql_error(conn_ptr));
    }
    else
    {
        res_ptr = mysql_store_result(conn_ptr);             //取出结果集
        if (res_ptr)
        {
            count = (unsigned long)mysql_num_rows(res_ptr);
        }
        mysql_free_result(res_ptr);
    }
    printf("\n%d\n",count);
    return count;
}

int delete(MYSQL *conn_ptr,int id,char *IMSI)
{
    int res;
    char *s = malloc(10 * sizeof(char));
    memset(s, 0, 10);
    sprintf(s,"%d",id);
    char sql[SQL_MAX_LENGTH] = "delete from record";
    strcat(sql," where ");
	strcat(sql," id ");
	strcat(sql," = '");
	strcat(sql, s);
	strcat(sql,"'");
	strcat(sql," and ");
	strcat(sql,"imsi");
	strcat(sql," = '");
	strcat(sql,IMSI);
	strcat(sql,"'");
    printf("the sql is %s\n", sql);
    res = mysql_query(conn_ptr,sql);
    printf("the res is %d\n", res);
    if (!res)
    {
        free(s);
        printf("delete %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
        if ((unsigned long)mysql_affected_rows(conn_ptr) == 0)
        {
            return 0;
        }
    }
    else
  	{
        free(s);
        fprintf(stderr, "Insert error %d: %sn",mysql_errno(conn_ptr),mysql_error(conn_ptr));
        return 0;
    }
    return 1;
}

