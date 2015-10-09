/******************************************************************************
  Copyright (C), 1996-2011, TP_LINK TECHNOLOGIES CO., LTD.

  File name:   cgic_local.h

  Description: Teamwork, CGI model. local function.

  Author:      Chenjiaxin

  History:
  ----------------------------------------------------------------------------

	V1.0, 2015-09-29, Chen Jiaxin		Create file.
*****************************************************************************/
#include "cgic_local.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_COMMAND    100
#define MAX_IMSI       20
#define MAX_PRODUCT_ID 20

static char imsi[MAX_IMSI] = {0};
static char product_id[MAX_PRODUCT_ID] = {0};

/******************************************************************************
  Function:    cgi_pack
  Description: pack file or folder.
  Input:
    (1)srcfolder: The absolute path of the source file/folder's folder
    (2)srcname  : The name of the source file/folder name.
    (3)dstfolder: The absolute path of the destination file's folder
    (4)dstname  : The name of the destinaton file.
  Output:
  Return: 0 for succeed, 1 for fail.
  Others:
*******************************************************************************/
int cgi_pack(const char *srcfolder, const char *srcname,
             const char *dstfolder, const char *dstname)
{
    if ( ( NULL == srcfolder ) || ( NULL == srcname )
      || ( NULL == dstfolder ) || ( NULL == dstname ) )
    {
        return 1;
    }

    char command[MAX_COMMAND];
    memset(command, 0, MAX_COMMAND);

    //strcat(command, "busybox tar -cf ");
    strcat(command, "tar -cf ");
    strcat(command, dstfolder);
    strcat(command, dstname);
    strcat(command, " -C ");
    strcat(command, srcfolder);
    strcat(command, " ");
    strcat(command, srcname);

    system(command);
    return 0;
}
/******************************************************************************
  Function:    cgi_unpack
  Description: unpack file
  Input:
    (1)srcfolder: The absolute path of the source file's folder
    (2)srcname  : The name of the source file's name.
    (3)dstfolder: The absolute path of the destination file's folder
  Output:
  Return: 0 for succeed, 1 for fail.
  Others:
*******************************************************************************/
int cgi_unpack(const char *srcfolder, const char *srcname,
               const char *dstfolder)
{
    if ( ( NULL == srcfolder ) || ( NULL == srcname )
      || ( NULL == dstfolder ) )
    {
        return 1;
    }

    char command[MAX_COMMAND];
    memset(command, 0, MAX_COMMAND);

    //strcat(command, "busybox tar -xf ");
    strcat(command, "tar -xf ");
    strcat(command, srcfolder);
    strcat(command, srcname);
    strcat(command, " -C ");
    strcat(command, dstfolder);

    system(command);
    return 0;
}
/******************************************************************************
  Function:    cgi_rmfile
  Description: remove file
  Input:
    (1)folder: The absolute path of the source file's folder
    (2)name  : The name of the source file's name.
  Output:
  Return: 0 for succeed, 1 for fail.
  Others:
*******************************************************************************/
int cgi_rmfile(const char *folder, const char *name)
{
    if ( ( NULL == folder ) || ( NULL == name) )
    {
        return 1;
    }

    char command[MAX_COMMAND];
    memset(command, 0, MAX_COMMAND);

    strcat(command, "rm -f ");
    strcat(command, folder);
    strcat(command, name);

    system(command);
    return 0;
}
/******************************************************************************
  Function:    cgi_rmfolder
  Description: remove folder
  Input:
    (1)folder: The absolute path of the source folder's folder
    (2)name  : The name of the source file's name.
  Output:
  Return: 0 for succeed, 1 for fail.
  Others:
*******************************************************************************/
int cgi_rmfolder(const char *folder, const char *name)
{
    if ( ( NULL == folder ) || ( NULL == name) )
    {
        return 1;
    }

    char command[MAX_COMMAND];
    memset(command, 0, MAX_COMMAND);

    strcat(command, "rm -rf ");
    strcat(command, folder);
    strcat(command, name);

    system(command);
    return 0;
}
/******************************************************************************
  Function:    cgi_mvfile
  Description: move file from src to dst
  Input:
    (1)srcfolder: The absolute path of the source file's folder
    (2)srcname  : The name of the source file's name.
    (3)dstfolder: The absolute path of the destination file's folder
  Output:
  Return: 0 for succeed, 1 for fail.
  Others:
*******************************************************************************/
int cgi_mvfile(const char *srcfolder, const char *srcname,
               const char *dstfolder)
{
    if ( ( NULL == srcfolder ) || ( NULL == srcname )
      || ( NULL == dstfolder ) )
    {
        return 1;
    }

    char command[MAX_COMMAND];
    memset(command, 0, MAX_COMMAND);

    strcat(command, "mv ");
    strcat(command, srcfolder);
    strcat(command, srcname);
    strcat(command, " ");
    strcat(command, dstfolder);

    system(command);
    return 0;
}
/******************************************************************************
  Function:    cgi_mvfolder
  Description: move folder from src to dst
  Input:
    (1)srcfolder: The absolute path of the source folder's folder
    (2)srcname  : The name of the source file's name.
    (3)dstfolder: The absolute path of the destination folder's folder
  Output:
  Return: 0 for succeed, 1 for fail.
  Others:
*******************************************************************************/
int cgi_mvfolder(const char *srcfolder, const char *srcname,
                 const char *dstfolder)
{
    if ( ( NULL == srcfolder ) || ( NULL == srcname )
      || ( NULL == dstfolder ) )
    {
        return 1;
    }

    char command[MAX_COMMAND];
    memset(command, 0, MAX_COMMAND);

    strcat(command, "mv ");
    strcat(command, srcfolder);
    strcat(command, srcname);
    strcat(command, " ");
    strcat(command, dstfolder);

    system(command);
    return 0;
}
/******************************************************************************
  Function:    cgi_get_imsi
  Description: return the ISMI buffer, if the buffer store nothing, return NULL.
  Input:
  Output:
  Return: The string storing the imsi.
  Others:
*******************************************************************************/
char *cgi_get_imsi(void)
{
    memset(imsi, 0, MAX_IMSI);

    /* Get the imsi */
    memset(imsi, 65, 15);
    /*FILE *fp = NULL;
    fp = popen("uci get sim.common_state.imsi", "r");
    if ( NULL == fp )
    {
        return NULL;
    }
    fread(imsi, sizeof(char), MAX_IMSI - 1, fp);
    pclose(fp);*/
    
    /* Replace the newline character */
    /*char *pnewline = strchr(imsi, '\n');
    if ( pnewline )
    {
        *pnewline = '\0';
    }*/

    if ( 0 == strlen(imsi) )
    {
        return NULL;
    }
    else
    {
        return imsi;
    }
}
/******************************************************************************
  Function:    cgi_get_product_id
  Description: return the product id, if the buffer store nothing, return NULL.
  Input:
  Output:
  Return: The string storing the product id.
  Others:
*******************************************************************************/
char *cgi_get_product_id(void)
{
    memset(product_id, 0, MAX_PRODUCT_ID);

    /* Get the product id */
    memset(product_id, 66, 8);
    /*
    FILE *fp = NULL;
    fp = popen("uci get product.info.product_id", "r");
    if ( NULL == fp )
    {
        return NULL;
    }
    fread(product_id, sizeof(char), MAX_PRODUCT_ID - 1, fp);
    pclose(fp);*/

    /* Replace the newline character */
    /*char *pnewline = strchr(product_id, '\n');
    if ( pnewline )
    {
        *pnewline = '\0';
    }*/

    if ( 0 == strlen(product_id) )
    {
        return NULL;
    }
    else
    {
        return product_id;
    }
}
