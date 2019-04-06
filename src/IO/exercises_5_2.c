/**
 * 《Linux/UNIX系统编程手册》第五章课后练习2
 */

#include <apue.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int fd;
    ssize_t num_write;
    mode_t mode = S_IRUSR | S_IWUSR;
    char *buf = "\n/*这是我用程序添加的一行注释。。。*/";
    char *file = "./exercises_5_2.c";

    if((fd = open(file, O_RDWR | O_CREAT | O_APPEND, mode)) < 0)
        err_sys("open file error");
    
    if(lseek(fd, 0, SEEK_SET) < 0)
        err_sys("seek error");
    
    num_write = write(fd, buf, strlen(buf));

    if(num_write < 0)
        err_sys("write error");
    if(num_write < strlen(buf))
        printf("write fewer bytes\n");
    
    printf("Write %lld bytes\n", (long long)num_write);
    exit(EXIT_SUCCESS);
}
/*这时我用程序添加的一行注释。。。*/