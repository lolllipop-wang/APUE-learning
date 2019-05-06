/**
 * 从/proc文件系统中读取max_pid 
 * 
 */
#include <apue.h>
#include <fcntl.h>

char buf[MAXLINE];

int main(int argc, char *argv[])
{

    int fd;
    int nread;
    int nwrite;
    if ( (fd = open("/proc/sys/kernel/pid_max", O_RDONLY)) < 0)
        err_sys("open file /proc/sys/kernel/pid_max error");
    
    while ( (nread = read(fd, buf, MAXLINE)) > 0)
        if ( (nwrite = write(STDOUT_FILENO, buf, nread)) < 0)
            err_sys("write error");
    
    if(nread < 0)
        err_sys("read error");
    
    exit(0);
}