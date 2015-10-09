/******************************************************************************
  Copyright (C), 1996-2011, TP_LINK TECHNOLOGIES CO., LTD.

  File name:   cgic_local.h

  Description: Teamwork, CGI model. local function.

  Author:      Chenjiaxin

  History:
  ----------------------------------------------------------------------------

	V1.0, 2015-09-29, Chen Jiaxin		Create file.
*****************************************************************************/
#ifndef CGIC_LOCAL_H_
#define CGIC_LOCAL_H_

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
  Function:    cgi_pack
  Description: pack file or folder.
  Input:
    (1)srcfolder: The absolute path of the source file/folder's folder
    (2)srcname  : The name of the source file/folder name.
    (3)dstfile  : The absolute path of the destination file
  Output:
  Return: 0 for succeed, -1 for fail.
  Others:
*******************************************************************************/
int cgi_pack(const char *srcfolder, const char *srcname, const char *dstfile);
/******************************************************************************
  Function:    cgi_unpack
  Description: unpack file
  Input:
    (1)srcfile  : The absolute path of the source file
    (2)dstfolder: The absolute path of the destination file's folder
  Output:
  Return: 0 for succeed, -1 for fail.
  Others:
*******************************************************************************/
int cgi_unpack(const char *srcfile, const char *dstfolder);
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
int cgi_rmfile(const char *file);
/******************************************************************************
  Function:    cgi_mvfile
  Description: move file from src to dst
  Input:
    (1)srcfile  : The absolute path of the source file
    (2)dstfolder: The absolute path of the destination file's folder
  Output:
  Return: 0 for succeed, 1 for fail.
  Others:
*******************************************************************************/
int cgi_mvfile(const char *srcfile, const char *dstfolder);
/******************************************************************************
  Function:    cgi_get_imsi
  Description: return the ISMI buffer, if the buffer store nothing, return NULL.
  Input:
  Output:
  Return: The string storing the imsi.
  Others:
*******************************************************************************/
char *cgi_get_imsi(void);
/******************************************************************************
  Function:    cgi_get_product_id
  Description: return the product id, if the buffer store nothing, return NULL.
  Input:
  Output:
  Return: The string storing the product id.
  Others:
*******************************************************************************/
char *cgi_get_product_id(void);
#ifdef __cplusplus
}
#endif

#endif /* CGIC_LOCAL_H_ */
