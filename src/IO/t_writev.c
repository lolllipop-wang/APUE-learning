#include <apue.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

#define MAXSIZE 100

char buf[]  = "#ABCDiov1\n";
char buf1[] = "$ABCDiov2\n";
char buf2[] = "&ABCDiov3\n"; 

int 
main(int argc, char *argv[])
{
    int fd;
    struct iovec iov[3];

    if(argc != 2)
        err_quit("Usage: %s file", argv[0]);
    
    ssize_t numWrite, totRequired = 0;

    if((fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND)) < 0)
        err_quit("open file error");

    iov[0].iov_base = buf;
    iov[0].iov_len = strlen(buf);
    totRequired += strlen(buf);

    iov[1].iov_base = buf1;
    iov[1].iov_len = strlen(buf1);
    totRequired += strlen(buf1);

    iov[2].iov_base = buf2;
    iov[2].iov_len = strlen(buf2);
    totRequired += strlen(buf2);

    if((numWrite = writev(fd, iov, 3)) < 0)
        err_quit("writev error");
    
    if(numWrite < totRequired)
        err_sys("Write fewer bytes than requested");
    
    printf("total bytes requested: %ld; bytes write: %ld\n", 
            (long)totRequired, (long)numWrite);
    exit(0);
}
