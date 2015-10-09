/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   test_name.c

 Description:

 Author:      hexiong

 Email:       hexiong@tp-link.net

--------------------------------------

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../socket_h/public_handle.h"

int main()
{
    char *filename = malloc(512 * sizeof(char));
    char imsi[16] = "asdfghjklqwerty";
    char product_id[9] = "73000001";
    char version_no[15] = "20150912000000";

    cons_file_name(imsi, product_id, version_no, &filename);
    printf("The finial result is %s\n",filename);
    return 0;
}
