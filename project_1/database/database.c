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
#include "/usr/include/mysql/mysql.h"
#include "../socket_h/message.h"
#include "../socket_h/public_handle.h"
#include "../socket_h/database.h"
#define EXIT_FAILURE 0
#define EXIT_SUCCESS 1

#define SQL_MAX_LENGTH 1000

#define IP "localhost"
#define DBUSER "root"
#define DBPASSWORD "tplink"
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



struct version_info *getList ( MYSQL *conn_ptr,char *IMSI,char *product_id )
{

	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    MYSQL_FIELD *fd;
	int res ,i ,j;
	int count = 0;
	int mark;
	struct version_info *ver_list = NULL;
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
    if (res) {
        printf("SELECT error:%s\n",mysql_error(conn_ptr));
    } else {
        res_ptr = mysql_store_result(conn_ptr);             //取出结果集
        if(res_ptr)
        {
            j = mysql_num_fields(res_ptr);  //j is the field in a table
            ver_list = malloc((unsigned long)mysql_num_rows(res_ptr) * sizeof(struct version_info));

            while((sqlrow = mysql_fetch_row(res_ptr)))  {   //依次取出记录
           		mark = count++;
            	ver_list[mark].id = atoi(sqlrow[0]);
            	strcpy(ver_list[mark].imsi,sqlrow[1]);
            	strcpy(ver_list[mark].product_id,sqlrow[2]);
            	strcpy(ver_list[mark].note,sqlrow[3]);
            }

            if (mysql_errno(conn_ptr))
            {
                fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));
            }
        }
        mysql_free_result(res_ptr);
    }
    return ver_list;
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

char *query(MYSQL *conn_ptr,int id,char *IMSI)
{

	char *s;
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
    }
    else
    {
        res_ptr = mysql_store_result(conn_ptr);

        if(res_ptr)
        {
			printf("%lu Rows\n",(unsigned long)mysql_num_rows(res_ptr));
			j = mysql_num_fields(res_ptr);
			file_path = mysql_fetch_row(res_ptr)[1];
			if (mysql_errno(conn_ptr)) {
				fprintf(stderr,"Retrive error:s\n",mysql_error(conn_ptr));
        }
        mysql_free_result(res_ptr);
    }
    return file_path;
}
int count(MYSQL *conn_ptr,char *IMSI,char *product_id)
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
	strcat(sql," and ");
	strcat(sql,"product_id");
	strcat(sql," = '");
	strcat(sql,product_id);
	strcat(sql,"'");

    res = mysql_query(conn_ptr,sql); //查询语句
    if (res) {
        printf("SELECT error:%s\n",mysql_error(conn_ptr));
    } else {
        res_ptr = mysql_store_result(conn_ptr);             //取出结果集
        if(res_ptr)
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
    char *s;
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
    res = mysql_query(conn_ptr,sql); 
    if(!res)
    { 
        printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(conn_ptr));
    }
    else
  	{
        fprintf(stderr, "Insert error %d: %sn",mysql_errno(conn_ptr),mysql_error(conn_ptr));
        return 0;
    }    
    return 1;
}




int main (int argc,char *argv[])
{
	
	MYSQL *conn_ptr;
	conn_ptr = mysql_init(NULL);
	
	if((conn_ptr = mysql_connect_ptr())!= NULL)
	{
		printf("Hello World");
       	//printf("\n%d\n",delete(conn_ptr,12,"111111111111222"));
        	
       	//printf("\n%d\n",add(conn_ptr,"777777777777777","2015_09_12_09_08","44444444","dsfasdfadsfasd","/home/happy_bear"));
        //	printf("\n%s\n",query(conn_ptr,9,"111111111111222"));
		/*
		int x = 0;
		for(x = 0;x<3;x++){
			printf("%d\n----",getList(conn_ptr,"111111111111111","11111111")[x].id);

		}
		*/
		//printf("%s",query(conn_ptr,15,"777777777777777"));
		count(conn_ptr,"777777777777777","44444444");
	}
	else
	{
		printf("Connect to Database failed");
	}
	return 0;
}
