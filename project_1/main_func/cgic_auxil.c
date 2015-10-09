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

/* macro for test */
#define CGIC_LOCAL_TEST_0    0

#define STATUS_DEALING        0
#define STATUS_SUCCEED        1
#define STATUS_FAILURE_LOCAL  2
#define STATUS_FAILURE_REMOTE 3

#define PACK_SRC_FOLDER "~/test/"
#define PACK_SRC_NAME   "config/"
#define PACK_DST_FOLDER "/tmp/"
#define PACK_DST_NAME   "config.tar"
#define PACK_DST_FILE   "/tmp/config.tar"

#define UNPACK_SRC_FOLDER "/tmp/"
#define UNPACK_SRC_NAME   "config.tar"
#define UNPACK_DST_FOLDER "/tmp/"

#define SERVER_IP  "127.0.0.1"

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
                    "\"dbid\":16,"
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

    /* Add TOWEB_STATUS */
    cJSON_AddNumberToObject(root, TOWEB_STATUS, 0);

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
#if CGIC_LOCAL_TEST_0
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
    cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_SUCCEED);
    output_cJSON(root);
    cJSON_Delete(root);
    root = NULL;
    return;
#endif

    /* getlist from server */
    char *imsi = cgi_get_imsi();
    char *product_id = cgi_get_product_id();
    if ( ( NULL == imsi ) || ( NULL == product_id ) )
    {
        /* Response to web: failure_local */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_FAILURE_LOCAL);
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
    int list_num = cgi_getlist(imsi, SERVER_IP, product_id, &plist);

    /* Response to web */
    if ( ( NULL == plist ) || ( -1 == list_num ) )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_FAILURE_REMOTE);
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
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_SUCCEED);
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
#if CGIC_LOCAL_TEST_0
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
    cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_SUCCEED);
    output_cJSON(root);
    cJSON_Delete(root);
    root = NULL;
    return;
#endif

    /* check for room to backup */
    char *imsi = cgi_get_imsi();
    char *product_id = cgi_get_product_id();
    if ( ( NULL == imsi ) || ( NULL == product_id ) )
    {
        /* Response to web: failure_local */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_FAILURE_LOCAL);
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
    int volnum = cgi_check(imsi, SERVER_IP);

    /* If check fails */
    if ( -1 == volnum )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_FAILURE_REMOTE);
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
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_FAILURE_REMOTE);
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
        cgi_pack(PACK_SRC_FOLDER, PACK_SRC_NAME, PACK_DST_FOLDER, PACK_DST_NAME);

        /* Backup config files */
        int flag_backup = cgi_backup(imsi, SERVER_IP, product_id, pinfo->remark,
                                     PACK_DST_FILE);

        /* If backup fail */
        if ( -1 == flag_backup )
        {
            /* Response to web: failure_remote */
            cJSON *root = cJSON_CreateObject();
            cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
            cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_FAILURE_REMOTE);
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
            cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_SUCCEED);
            output_cJSON(root);
            cJSON_Delete(root);
            root = NULL;
            /* end of: Response to web: succeed */
        }
        /* Delete local packed config file */
        cgi_rmfile(PACK_DST_FOLDER, PACK_DST_NAME);
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
#if CGIC_LOCAL_TEST_0
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
    cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_SUCCEED);
    output_cJSON(root);
    cJSON_Delete(root);
    root = NULL;
    return;
#endif

#if CGIC_LOCAL_TEST
    printf("dbid: %d\n", pinfo->dbid);
#endif
    char *imsi = cgi_get_imsi();


    /* Request file from server */
    int flag_recover = cgi_recover(pinfo->dbid, SERVER_IP, imsi);
    /* If recover fail */
    if ( -1 == flag_recover )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_FAILURE_REMOTE);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: failure_remote */
    }
    else
    {
        /* Unpack file */
        cgi_unpack(UNPACK_SRC_FOLDER, UNPACK_SRC_NAME, UNPACK_DST_FOLDER);

        /* Rewrite files */
        cgi_rmfolder(PACK_SRC_FOLDER, PACK_SRC_NAME);
        cgi_mvfolder(UNPACK_DST_FOLDER, PACK_SRC_NAME, PACK_SRC_FOLDER);
        cgi_rmfile(UNPACK_SRC_FOLDER, UNPACK_SRC_NAME);

        /* Response to web: succeed*/
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_SUCCEED);
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
#if CGIC_LOCAL_TEST_0
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
    cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_SUCCEED);
    output_cJSON(root);
    cJSON_Delete(root);
    root = NULL;
    return;
#endif

#if CGIC_LOCAL_TEST
    printf("dbid: %d\n", pinfo->dbid);
#endif

    /* Get IMSI */
    char *imsi = cgi_get_imsi();

    /* Request server to delete */
    int flag_delete = cgi_delete(pinfo->dbid, SERVER_IP, imsi);

    /* If delete fail */
    if ( -1 == flag_delete )
    {
        /* Response to web: failure_remote */
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, TOWEB_ACTION, pinfo->action);
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_FAILURE_REMOTE);
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
        cJSON_AddNumberToObject(root, TOWEB_STATUS, STATUS_SUCCEED);
        output_cJSON(root);
        cJSON_Delete(root);
        root = NULL;
        /* end of: Response to web: succeed */
    }
}
