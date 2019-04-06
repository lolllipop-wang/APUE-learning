/**
 * 《Linux/UNIX系统编程手册》第五章课后练习2
 */
#include <apue.h>
#include <fcntl.h>

#define MAXBUFFSIZE 4096

char buf[MAXBUFFSIZE];

int
main(int argc, char *argv[])
{
    if(argc < 2)
        err_quit("Usage: %s file", argv[0]);
    
    int fd, newfd;
    ssize_t step = 100;
    ssize_t num_read;
    if((fd = open(argv[1], O_RDONLY)) < 0)
        err_sys("open file error");
    printf("###########\n");
    printf("fd read begin\n");

    num_read = read(fd, buf, step);

    if(num_read < 0)
        err_sys("fd read error");
    if(num_read < step)
        printf("fd read fewer bytes\n");
    printf("fd read %lld bytes\n", (long long)num_read);
    buf[num_read] = '\0';
    printf("fd read:\n%s\nfd read end\n\n", buf);
    printf("###########\n");
    
    if((newfd = dup(fd)) < 0)
        err_sys("dup error");
    
    printf("###########\n");
    printf("new fd read begin\n");

    num_read = read(newfd, buf, step);

    if(num_read < 0)
        err_sys("new fd read error");
    if(num_read < step)
        printf("new fd read fewer bytes\n");
    printf("new fd read %lld bytes\n", (long long)num_read);
    buf[num_read] = '\0';
    printf("new fd read:\n%s\nnew fd read end\n\n", buf);
    printf("###########\n");
    exit(EXIT_SUCCESS);
}