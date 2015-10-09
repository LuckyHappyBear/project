#include <stdio.h>
#include "cgic_local.h"

#include <sys/stat.h>
#include <unistd.h>

#define TEST_PACK   0
#define TEST_UNPACK 0
#define TEST_RMFILE 1
#define TEST_MVFILE 0
#define TEST_IMSI   0
#define TEST_PROID  0

#define PACK_SRC_FOLDER "/home/chenjiaxin/tartest"
#define PACK_SRC_NAME   "config"
#define PACK_DST_FILE   "/tmp/config.tar"

#define UNPACK_SRC_FILE   "/tmp/config.tar"
#define UNPACK_DST_FOLDER "/home/chenjiaxin/tartest"

#define RMFILE "/home/chenjiaxin/tartest/config"
//#define RMFILE "/tmp/config.tar"

#define MVFILE_SRC_FILE   "/home/chenjiaxin/tartest"
#define MVFILE_DST_FOLDER "/tmp"

int main(void)
{
#if TEST_PACK
    int flag = cgi_pack(PACK_SRC_FOLDER, PACK_SRC_NAME, PACK_DST_FILE);
    if ( 0 == flag )
    {
        puts("Pack succeeds");
    }
    else
    {
        puts("Pack fails");
    }
#endif

#if TEST_UNPACK
    int flag = cgi_unpack(UNPACK_SRC_FILE, UNPACK_DST_FOLDER);
    if ( 0 == flag )
    {
        puts("Unpack succeeds");
    }
    else
    {
        puts("Unpack fails");
    }
#endif

#if TEST_RMFILE
    int flag = cgi_rmfile(RMFILE);
    if ( 0 == flag )
    {
        puts("Rmfile succeeds");
    }
    else
    {
        puts("Rmfile fails");
    }
#endif

#if TEST_MVFILE
    int flag = cgi_mvfile(MVFILE_SRC_FILE, MVFILE_DST_FOLDER);
    if ( 0 == flag )
    {
        puts("Mvfile succeeds");
    }
    else
    {
        puts("Mvfile fails");
    }
#endif

#if TEST_IMSI
#endif

#if TEST_PROID
#endif
    return 0;
}
