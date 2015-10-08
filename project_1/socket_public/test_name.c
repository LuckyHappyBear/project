/*************************************************************************
 Copyright (C), 2015, TP-LINK TECHNOLOGIES CO., LTD.

 File Name:   test_name.c

 Description:

 Author:      hexiong

 Email:       hexiong@tp-link.net

--------------------------------------

*************************************************************************/

#include <stdio.h>
#include "../socket_h/public_handle.h"

int main()
{
    char filename[512] = {0};
    char imsi[16] = "asdfghjklqwerty";
    char product_id[9] = "73000001";
    char version_no[15] = "20150912000000";

    strcpy(filename, cons_file_name(imsi, product_id, version_no));

    printf("The file name is %s\n", filename);
    return 0;
}
