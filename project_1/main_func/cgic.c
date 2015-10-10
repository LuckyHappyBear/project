/******************************************************************************
  Copyright (C), 1996-2011, TP_LINK TECHNOLOGIES CO., LTD.

  File name:   cgic.c

  Description: Teamwork, CGI model.

  Author:      Chenjiaxin

  History:
  ----------------------------------------------------------------------------

	V1.0, 2015-09-29, Chen Jiaxin		Create file.
*****************************************************************************/
#define CGIC_LOCAL_TEST_0 0
#define CGIC_LOCAL_TEST_1 0
#define CGIC_LOCAL_TEST_2 1

#include <stdlib.h>
#include <stdio.h>
#include "cgic_auxil.h"

int main(void)
{
#if CGIC_LOCAL_TEST_0
    /* Get input from web */
    char *pinput = get_input();
    //char *pinput = get_input_test();
    if ( NULL == pinput )
    {
        return 0;
    }

    /* Parse input from web */
    struct data_from_web info;
    /* Parse input fail */
    if ( 0 != parse_input(pinput, &info) )
    {
        free(pinput);
        return 0;
    }

    /* Deal with info from web, deliver task according to "action" */
    switch(info.action)
    {
    case ACTION_GETLIST:
        action_getlist(&info);
        break;
    case ACTION_BACKUP:
        action_backup(&info);
        break;
    case ACTION_RECOVER:
        action_recover(&info);
        break;
    case ACTION_DELETE:
        action_delete(&info);
        break;
    case ACTION_SETIP:
        action_setip(&info);
        break;
    case ACTION_GETIP:
        action_getip(&info);
        break;
    default:
        break;
    }

    /* Output json string */

    free(pinput);
    return 0;

#elif CGIC_LOCAL_TEST_1
    /* Get input from web */
    char *pinput = get_input_test();

    /* Parse input from web */
    struct data_from_web info;
    /* Parse input fail */
    if ( 0 != parse_input(pinput, &info) )
    {
        puts("parse input fail");
        free(pinput);
        return 0;
    }

    /* Deal with info from web */
    printf("module = %s\n", info.module);
    printf("action = %d\n", info.action);
    printf("dbid   = %d\n", info.dbid);
    printf("remark = %s\n", info.remark);

    /* Output json string */
    output_test();
    puts("output_test finished");

    free(pinput);
    return 0;

#elif CGIC_LOCAL_TEST_2
    /* Get input from web */
    char *pinput = get_input_test();
    if ( NULL == pinput )
    {
        puts("No input string");
        return 0;
    }

    /* Parse input from web */
    struct data_from_web info;
    /* Parse input fail */
    if ( 0 != parse_input(pinput, &info) )
    {
        puts("Input fail");
        free(pinput);
        return 0;
    }
    /* Deal with info from web */
    puts("Parse result");
    printf("module = %s\n", info.module);
    printf("action = %d\n", info.action);
    printf("dbid   = %d\n", info.dbid);
    printf("remark = %s\n", info.remark);
    putchar('\n');

    /* Deal with info from web, deliver task according to "action" */
    puts("Dealing");
    switch(info.action)
    {
    case ACTION_GETLIST:
        puts("Action: getlist");
        action_getlist(&info);
        break;
    case ACTION_BACKUP:
        puts("Action: backup");
        action_backup(&info);
        break;
    case ACTION_RECOVER:
        puts("Action: recover");
        action_recover(&info);
        break;
    case ACTION_DELETE:
        puts("Action: delete");
        action_delete(&info);
        break;
    case ACTION_SETIP:
        puts("Action: setip");
        action_setip(&info);
        break;
    case ACTION_GETIP:
        puts("Action: getip");
        action_getip(&info);
        break;
    default:
        puts("Unexpected action");
        break;
    }

    /* Output json string */

    free(pinput);
    return 0;

#else /* The normal function */

    /* Get input from web */
    char *pinput = get_input();
    if ( NULL == pinput )
    {
        return 0;
    }

    /* Parse input from web */
    struct data_from_web info;
    /* Parse input fail */
    if ( 0 != parse_input(pinput, &info) )
    {
        free(pinput);
        return 0;
    }

    /* Deal with info from web, deliver task according to "action" */
    switch(info.action)
    {
    case ACTION_GETLIST:
        action_getlist(&info);
        break;
    case ACTION_BACKUP:
        action_backup(&info);
        break;
    case ACTION_RECOVER:
        action_recover(&info);
        break;
    case ACTION_DELETE:
        action_delete(&info);
        break;
    case ACTION_SETIP:
        action_setip(&info);
        break;
    case ACTION_GETIP:
        action_getip(&info);
        break;
    default:
        break;
    }

    /* Output json string */

    free(pinput);
    return 0;

#endif
}

