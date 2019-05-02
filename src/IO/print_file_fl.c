/**
 * 打印文件标志说明
 * example: O_RDONLY:   read only
 *          O_WRONLY:   write only
 *          O_RDWR:     read and write
 *          O_APPEND:   append
 *          O_NONBLOCK: nonblock
 *          O_SYNC:     sync
 */
#include <apue.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int val;

    if(argc != 2)
        err_quit("usage: %s <descriptor#>");
    
    val = fcntl(atoi(argv[1]), F_GETFL, 0);
    if(val < 0)
        err_sys("%s: fcntl F_GETFL error", argv[0]);
    
    /**
     * O_RDONLY, O_WRONLY, O_RDWR不能同时存在
     * 
     */
    switch (val & O_ACCMODE)
    {
        case O_RDONLY:
            printf("read only");
            break;

        case O_WRONLY:
            printf("write only");
            break;
        
        case O_RDWR:
            printf("read and write");
            break;
        
        default:

            printf("unkown access mode");
            break;
    }

    if(val & O_APPEND)
        printf(", append");
    if(val & O_SYNC)
        printf(", sync");
    if(val & O_NONBLOCK)
        printf(", nonblock");
#if !defined(_POSIX_X_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if(val & O_FSYNC)
        printf(", fsync");
#endif
    printf("\n");
    exit(0);
}