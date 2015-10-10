/******************************************************************************
  Copyright (C), 1996-2011, TP_LINK TECHNOLOGIES CO., LTD.

  File name:   cgic_auxil.c

  Description: Teamwork, CGI model. Auxiliary functions

  Author:      Chenjiaxin

  History:
  ----------------------------------------------------------------------------

	V1.0, 2015-09-29, Chen Jiaxin		Create file.
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../cJSON/cJSON.h"
#include "cgic_auxil.h"
#include "../local_func/cgic_local.h"
#include "../socket_h/cgic_client.h"
#include "../socket_h/message.h"
#include "../socket_h/public_handle.h"

#define RESULT_DEALING               0
#define RESULT_SUCCEED               1
#define RESULT_FAILURE_LOCAL         2
#define RESULT_FAILURE_REMOTE        3
#define RESULT_FAILURE_REMOTE_CHECK  3
#define RESULT_FAILURE_REMOTE_FULL   4
#define RESULT_FAILURE_REMOTE_BACKUP 5

#define PACK_SRC_FOLDER "/app/"
#define PACK_SRC_NAME   "config"
#define PACK_SRC_FILE   "/app/config"
#define PACK_DST_FILE   "/tmp/config.tar"

#define UNPACK_SRC_FILE "/tmp/config.tar"
#define UNPACK_DST_FOLDER "/tmp/"
#define UNPACK_DST_FILE "/tmp/config"

//#define SERVER_IP  "127.0.0.1"

#define DEFAULT_VOLUME 10

/******************************************************************************
  Function:    get_input_test
  Description: Test for local.
  Input:
  Output:
  Return: A pointer to a string storing the json data.
  Others:
*******************************************************************************/
char *get_input_test(void)
{
    char input[] = "{\"module\":\"backup\","
                    "\"action\":3,"
                    "\"dbid\":22,"
                    "\"remark\":\"nothing\"}";
    char *pout = (char *)malloc( 1 + sizeof(char) * strlen(input) );
    memset(pout, 0, 1 + sizeof(char) * strlen(input) );
    strcpy(pout,input);
    printf("input = %s\n\n", input);
    return pout;
}
/******************************************************************************
  Function:    get_input
  Description: Get input from web. Web transfer data by method "POST", and data
               structure of JSON.
  Input:
  Output:
  Return: A pointer to a string storing the json data.
  Others:
    (1)If the transfer method is GET or nothing , return NULL pointer.
*******************************************************************************/
char *get_input(void)
{
    char *pmethod = getenv("REQUEST_METHOD");

    /* Do nothing if the method is nonexistence or is "GET" */
    if ( ( NULL == pmethod ) || ( !strcmp(pmethod, "GET") ) )
    {
        return NULL;
    }
    /* Deal with method "POST" */
    else if ( !strcmp(pmethod, "POST") )
    {
        /* Get the length of the POST data */
        int datalen = atoi( getenv("CONTENT_LENGTH") );

        /* If no data */
        if ( 0 == datalen )
        {
            return NULL;
        }
        else
        {
            /* buffer to store the input from web */
            int buflen = 1 + sizeof(char) * datalen;
            char *pinput = (char *)malloc( buflen );
            if ( NULL == pinput )
            {
                //perror("No room for input from web to cgi");
                assert( NULL != pinput );
            }
            memset(pinput, 0, buflen);

            /* Read input to buffer */
            fread(pinput, sizeof(char), datalen, stdin);

            return pinput;
        }
    }
    /* Deal with exception method */
    else
    {
        return NULL;
    }
}
/******************************************************************************
  Function:    parse_input
  Description: parse input data from web whose data structure is json, and store
               the value in the structure for following operation.
  Input:
    (1)pinput: A string with data whose structure is json.
    (2)pstru : A data structure to store the command from web.
  Output:
  Return:
    (1)0 for succeed, non-zero for fail
  Others:
*******************************************************************************/
int parse_input(char *pinput, struct data_from_web *pstru)
{
    /* Input check */
    if ( NULL == pinput )
    {
        return 1;
    }
    if ( NULL == pstru )
    {
        return 2;
    }

    /* Initialize *pstru */
    pstru->action = 0;
    pstru->dbid   = 0;
    memset(pstru->remark, 0, MAX_REMARK);
    memset(pstru->module, 0, MAX_MODULE);
    memset(pstru->ip,     0, MAX_IP);

    /* Parse input string to cjson structure */
    cJSON *pjson_web_to_cgi = cJSON_Parse(pinput);

    /* cJSON parse fail */
    if ( NULL == pjson_web_to_cgi )
    {
#if CGIC_LOCAL_TEST
        printf("Error befor: [%s]\n", cJSON_GetErrorPtr());
#endif
        return 3;
    }
    /* cJSON parse succeed */
    else
    {
        cJSON *pjson_val = NULL;

        /* parse WEB_ACTION */
        pjson_val = cJSON_GetObjectItem(pjson_web_to_cgi, WEB_ACTION);
        if ( NULL != pjson_val )
        {
            pstru->action = pjson_val->valueint;
        }

        /* parse WEB_DBID */
        pjson_val = cJSON_GetObjectItem(pjson_web_to_cgi, WEB_DBID);
        if ( NULL != pjson_val )
        {
            pstru->dbid = pjson_val->valueint;
        }

        /* parse WEB_REMARK */
        pjson_val = cJSON_GetObjectItem(pjson_web_to_cgi, WEB_REMARK);
        if ( NULL != pjson_val )
        {
            strcpy(pstru->remark, pjson_val->valuestring);
        }

        /* parse WEB_MODULE */
        pjson_val = cJSON_GetObjectItem(pjson_web_to_cgi, WEB_MODULE);
        if ( NULL != pjson_val )
        {
            strcpy(pstru->module, pjson_val->valuestring);
        }

        /* parse WEB_IP */
        pjson_val = cJSON_GetObjectItem(pjson_web_to_cgi, WEB_IP);
        if ( NULL != pjson_val )
        {
            strcpy(pstru->ip, pjson_val->valuestring);
        }

        cJSON_Delete(pjson_web_to_cgi);
        return 0;
    }
}
/******************************************************************************
  Function:    output_test
  Description: Output cgi test.
  Input:
  Output:
  Return:
  Others:
*******************************************************************************/
void output_test(void)
{
    /* create JSON object */
    cJSON *root = cJSON_CreateObject();

    /* Add TOWEB_ACTION */
    cJSON_AddNumberToObject(root, TOWEB_ACTION, 1);

    /* Add TOWEB_RESULT */
    cJSON_AddNumberToObject(root, TOWEB_RESULT, 0);

    /* Add TOWEB_USEDVOLUME */
    cJSON_AddNumberToObject(root, TOWEB_USEDVOLUME, 3);

    /* Add TOWEB_FULLVOLUME */
    cJSON_AddNumberToObject(root, TOWEB_FULLVOLUME, 10);

    /* Add TOWEB_ITEM */
    cJSON *js_item = cJSON_CreateArray();
    cJSON_AddItemToObject(root, TOWEB_ITEM, js_item);

    /* Add All TOWEB_ITEM including TOWEB_DBID, TOWEB_TIMESTAMP, TOWEB_REMARK */
    int dbid[3] = { 1234, 2345, 3456 };
    char *timestamp[3] =
    {
        "2015_09_29_08_30_00",
        "2015_09_29_09_30_00",
        "2015_09_29_10_30_00"
    };
    char *remark[3] =
    {
        "Example remark 1",
        "Example remark 2",
        "Example remark 3"
    };
    int i = 0;
    for ( i = 0; i < 3; i++ )
    {
        cJSON *js_elemobj = cJSON_CreateObject();
        cJSON_AddNumberToObject(js_elemobj, TOWEB_DBID, dbid[i]);
        cJSON_AddStringToObject(js_elemobj, TOWEB_TIMESTAMP, timestamp[i]);
        cJSON_AddStringToObject(js_elemobj, TOWEB_REMARK, remark[i]);

        cJSON_AddItemToArray(js_item, js_elemobj);
    }

    char *out1 = cJSON_Print(root);
    char *out2 = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    printf("%s", out1);
    putchar('\n');
    printf("%s", out2);
    putchar('\n');
    free(out1);
    free(out2);
}
/******************************************************************************
  Function:    output_cJSON
  Description: output cJSON data with MIME header
  Input:
    (1)root: root for cJSON structure
  Output:
  Return:
  Others:
*******************************************************************************/
static
void output_cJSON(cJSON *root)
{
    char *out = cJSON_PrintUnformatted(root);
    printf("Content-Type: text/html\n");
    printf("Content-Length: %d\n\n", (int)strlen(out));
    printf("%s",out);
    free(out);
    out = NULL;
}
/******************************************************************************
  Function:    action_getlist
  Description: Deal with "getlist" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_getlist(struct data_from_web *pinfo)
{
    /* getlist from server */
    char *imsi = cgi_get_imsi();
    char *product_id = cgi_get_product_id();
    char *server_ip = cgi_getip();
    if ( ( NULL == imsi ) || ( NULL == product_id ) || ( NULL == server_ip ) )
    {
        /* Response to web: failure_local */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_local */
        return;
    }

#if CGIC_LOCAL_TEST
    printf("imsi: %s\n", imsi);
    printf("product_id: %s\n", product_id);
#endif

    /* Get list from remote server */
    struct version_info *plist = NULL;
    int list_num = cgi_getlist(imsi, server_ip, product_id, &plist);

    /* Response to web */
    if ( ( NULL == plist ) || ( -1 == list_num ) )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_REMOTE);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_remote */
    }
    else
    {
        /* Response to web: succeed */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_SUCCEED);
        cJSON_AddNumberToObject(root, TOWEB_USEDVOLUME, list_num);
        cJSON_AddNumberToObject(root, TOWEB_FULLVOLUME, DEFAULT_VOLUME);

        cJSON *js_item = cJSON_CreateArray();
        cJSON_AddItemToObject(root, TOWEB_ITEM, js_item);
        int i = 0;
        for ( i = 0; i < list_num; i++ )
        {
            cJSON *js_elemobj = cJSON_CreateObject();
            cJSON_AddNumberToObject(js_elemobj, TOWEB_DBID,      plist[i].id);
            cJSON_AddStringToObject(js_elemobj, TOWEB_TIMESTAMP, plist[i].version_no);
            cJSON_AddStringToObject(js_elemobj, TOWEB_REMARK,    plist[i].note);
            cJSON_AddItemToArray(js_item, js_elemobj);
        }

        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: succeed */
    }
    if ( NULL != plist )
    {
        free(plist);
        plist = NULL;
    }
}
/******************************************************************************
  Function:    action_backup
  Description: Deal with "backup" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_backup(struct data_from_web *pinfo)
{
    /* check for room to backup */
    char *imsi = cgi_get_imsi();
    char *product_id = cgi_get_product_id();
    char *server_ip = cgi_getip();
    if ( ( NULL == imsi ) || ( NULL == product_id ) || ( NULL == server_ip ) )
    {
        /* Response to web: failure_local */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_local */
        return;
    }

#if CGIC_LOCAL_TEST
    printf("imsi: %s\n", imsi);
    printf("product_id: %s\n", product_id);
#endif

    /* Check the used volume of the backup files */
    int volnum = cgi_check(imsi, server_ip);

    /* If check fails */
    if ( -1 == volnum )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_REMOTE_CHECK);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_remote */
    }
    /* If no room for more backup file in server */
    else if ( DEFAULT_VOLUME == volnum )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_REMOTE_FULL);
        cJSON_AddNumberToObject(root, TOWEB_USEDVOLUME, volnum);
        cJSON_AddNumberToObject(root, TOWEB_FULLVOLUME, DEFAULT_VOLUME);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_remote */
    }
    else
    {
        /* Pack config files */
        int flag_pack = cgi_pack(PACK_SRC_FOLDER, PACK_SRC_NAME, PACK_DST_FILE);
        /* If pack fail */
        if ( -1 == flag_pack )
        {
            /* Response to web: failure_local */
            cJSON *root = cJSON_CreateObject();
            cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
            cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
            output_cJSON(root);
            cJSON_Delete(root);
            root = NULL;
            /* end of: Response to web: failure_local */
            return;
        }

        /* Backup config files */
        int flag_backup = cgi_backup(imsi, server_ip, product_id, pinfo->remark, PACK_DST_FILE);
        /* If backup fail */
        if ( -1 == flag_backup )
        {
            /* Response to web: failure_remote */
            cJSON *root = cJSON_CreateObject();
            cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
            cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_REMOTE_BACKUP);
            cJSON_AddNumberToObject(root, TOWEB_USEDVOLUME, volnum);
            cJSON_AddNumberToObject(root, TOWEB_FULLVOLUME, DEFAULT_VOLUME);
            output_cJSON(root);
            cJSON_Delete(root);
            root = NULL;
            /* end of: Response to web: failure_remote */
        }
        else
        {
            /* Response to web: succeed */
            cJSON *root = cJSON_CreateObject();
            cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
            cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_SUCCEED);
            output_cJSON(root);
            cJSON_Delete(root);
            root = NULL;
            /* end of: Response to web: succeed */
        }
        /* Delete local packed config file */
        cgi_rmfile(PACK_DST_FILE);
    }
}
/******************************************************************************
  Function:    action_recover
  Description: Deal with "recover" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_recover(struct data_from_web *pinfo)
{
#if CGIC_LOCAL_TEST
    printf("dbid: %d\n", pinfo->dbid);
#endif
    char *imsi = cgi_get_imsi();
    char *server_ip = cgi_getip();
    if ( ( NULL == imsi ) || ( NULL == server_ip ) )
    {
        /* Response to web: failure_local */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_local */
        return;
    }

    /* Request file from server */
    int flag_recover = cgi_recover(pinfo->dbid, server_ip, imsi);
    /* If recover fail */
    if ( -1 == flag_recover )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_REMOTE);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_remote */
    }
    else
    {
        /* Unpack file */
        int flag_unpack = cgi_unpack(UNPACK_SRC_FILE, UNPACK_DST_FOLDER);
        /* If unpack fails */
        if ( -1 == flag_unpack )
        {
            /* Response to web: failure_local */
            cJSON *root = cJSON_CreateObject();
            cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
            cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
            output_cJSON(root);
            cJSON_Delete(root);
            root = NULL;
            /* end of: Response to web: failure_local */
            return;
        }

        /* Rewrite files */
        cgi_rmfile(PACK_SRC_FILE);
        int flag_mv = cgi_mvfile(UNPACK_DST_FILE, PACK_SRC_FOLDER);
        /* If mvfile fails */
        if ( -1 == flag_mv )
        {
            /* Response to web: failure_local */
            cJSON *root = cJSON_CreateObject();
            cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
            cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
            output_cJSON(root);
            cJSON_Delete(root);
            root = NULL;
            /* end of: Response to web: failure_local */
            return;
        }
        cgi_rmfile(UNPACK_SRC_FILE);

        /* Response to web: succeed*/
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_SUCCEED);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: succeed */
    }
}
/******************************************************************************
  Function:    action_delete
  Description: Deal with "delete" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_delete(struct data_from_web *pinfo)
{
#if CGIC_LOCAL_TEST
    printf("dbid: %d\n", pinfo->dbid);
#endif

    /* Get IMSI */
    char *imsi = cgi_get_imsi();
    char *server_ip = cgi_getip();
    if ( ( NULL == imsi ) || ( NULL == server_ip ) )
    {
        /* Response to web: failure_local */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_local */
        return;
    }

    /* Request server to delete */
    int flag_delete = cgi_delete(pinfo->dbid, server_ip, imsi);

    /* If delete fail */
    if ( -1 == flag_delete )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_REMOTE);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_remote */
    }
    else
    {
        /* Response to web: succeed */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_SUCCEED);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: succeed */
    }
}
/******************************************************************************
  Function:    action_setip
  Description: Deal with "setip" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_setip(struct data_from_web *pinfo)
{
    int flag = cgi_setip(pinfo->ip);
    /* If fail to set ip */
    if ( -1 == flag )
    {
        /* Response to web: failure_local */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_local */
    }
    else
    {
        /* Response to web: succeed */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_SUCCEED);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: succeed */
    }
}
/******************************************************************************
  Function:    action_getip
  Description: Deal with "getip" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_getip(struct data_from_web *pinfo)
{
    char *server_ip  = cgi_getip();
    /* If fail to get server ip */
    if ( NULL == server_ip )
    {
        /* Response to web: failure_local */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_FAILURE_LOCAL);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_local */
    }
    else
    {
        /* Response to web: succeed */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_RESULT, RESULT_SUCCEED);
        cJSON_AddStringToObject(root, TOWEB_IP,     server_ip);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: succeed */
    }
}
