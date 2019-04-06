#include <apue.h>
#include <stdlib.h>

#define MAXSIZE 100

char buf[MAXSIZE];
char str[MAXSIZE];

int
main(int argc, char *argv[])
{

    int fd;
    int numRead = 0, numWrite = 0;
    char template[] = "/tmp/somethingXXXXXX";

    fd = mkstemp(template);

    if(fd == -1)
        err_sys("create temp file error");
    
    printf("Generated file name was: %s\n", template);

    printf("please input a buffer: ");
    scanf("%s", buf);
    int length = strlen(buf);
    if((numWrite = write(fd, buf, length)) < length) {
        unlink(template);
        err_sys("write error");
    }
        
    /**
     * 还未解决的问题：
     * 为什么可以使用write写入，但是不能用read读出
     * read读出是0字节
     * 
     */
    if((numRead = read(fd, str, numWrite)) < numWrite) {
        unlink(template);
        err_sys("read error");
    }

    str[numRead] = '\0';
    printf("Buffer: %s\n", str);

    unlink(template);

    if(close(fd) == -1) 
        err_sys("close file error");

    exit(EXIT_SUCCESS);
}