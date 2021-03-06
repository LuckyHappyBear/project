/******************************************************************************
  Copyright (C), 1996-2011, TP_LINK TECHNOLOGIES CO., LTD.

  File name:   cgic_auxil.h

  Description: Teamwork, CGI model. Auxiliary functions

  Author:      Chenjiaxin

  History:
  ----------------------------------------------------------------------------

	V1.0, 2015-09-29, Chen Jiaxin		Create file.
*****************************************************************************/
#ifndef CGIC_AUXIL_H_
#define CGIC_AUXIL_H_

#define CGIC_LOCAL_TEST   1

/* Data from web to CGI */
#define WEB_MODULE "module"
#define WEB_ACTION "action"
#define WEB_REMARK "remark"
#define WEB_DBID   "dbid"
#define WEB_IP     "ip"
/* Data from web: Action */
#define ACTION_GETLIST 0
#define ACTION_BACKUP  1
#define ACTION_RECOVER 2
#define ACTION_DELETE  3
#define ACTION_SETIP   4
#define ACTION_GETIP   5
/* Data structure from web to CGI */
#define MAX_REMARK 100
#define MAX_MODULE 100
#define MAX_IP     20
struct data_from_web
{
    int  action;
    int  dbid;
    char module[MAX_MODULE];
    char remark[MAX_REMARK];
    char ip[MAX_IP];
};

/* Data from CGI to web */
#define TOWEB_ACTION      "action"
#define TOWEB_RESULT      "result"
#define TOWEB_USEDVOLUME  "usedvolume"
#define TOWEB_FULLVOLUME  "fullvolume"
#define TOWEB_ITEM        "item"
#define TOWEB_DBID        "dbid"
#define TOWEB_TIMESTAMP   "timestamp"
#define TOWEB_REMARK      "remark"
#define TOWEB_IP          "ip"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
  Function:    get_input
  Description: Get input from web(stdin).
  Input:
  Output:
  Return: A pointer to a string storing the json data.
  Others:
*******************************************************************************/
char *get_input(void);

/******************************************************************************
  Function:    parse_input
  Description: parse input data from web whose data structure is json, and store
               the value in the structure for following operation.
  Input:
    (1)pinput: A string with data whose structure is json.
    (2)pstru : A data structure to store the command from web.
  Output:
  Return:
    (1) 0 for succeed
    (2) 1 for no input string
    (3) 2 for no output data strucutre
    (4) 3 for json parse fail
  Others:
*******************************************************************************/
int parse_input(char *pinput, struct data_from_web *pstru);
/******************************************************************************
  Function:    get_input_test
  Description: Test for local.
  Input:
  Output:
  Return: A pointer to a string storing the json data.
  Others:
*******************************************************************************/
char *get_input_test(void);
/******************************************************************************
  Function:    output_test
  Description: Output cgi test.
  Input:
  Output:
  Return:
  Others:
*******************************************************************************/
void output_test(void);
/******************************************************************************
  Function:    action_getlist
  Description: Deal with "getlist" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_getlist(struct data_from_web *pinfo);
/******************************************************************************
  Function:    action_backup
  Description: Deal with "backup" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_backup(struct data_from_web *pinfo);
/******************************************************************************
  Function:    action_recover
  Description: Deal with "recover" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_recover(struct data_from_web *pinfo);
/******************************************************************************
  Function:    action_delete
  Description: Deal with "delete" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_delete(struct data_from_web *pinfo);
/******************************************************************************
  Function:    action_setip
  Description: Deal with "setip" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_setip(struct data_from_web *pinfo);
/******************************************************************************
  Function:    action_getip
  Description: Deal with "getip" request from web
  Input:
    (1)pinfo: A pointer to the structure storing the data from web
  Output:
  Return:
  Others:
*******************************************************************************/
void action_getip(struct data_from_web *pinfo);
#ifdef __cplusplus
}
#endif

#endif /* CGIC_AUXIL_H_ */
