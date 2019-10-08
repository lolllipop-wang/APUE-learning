#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXSIZE 100

char buf[MAXSIZE];
char str[MAXSIZE];

int main(int argc, char *argv[]) {

    int fd;
    size_t numRead = 0, numWrite = 0;
    char templates[] = "/tmp/somethingXXXXXX";

    fd = mkstemp(templates);

    if (fd == -1) {
        fprintf(stderr, "create temp file error\n");
        exit(1);
    }

    printf("Generated file name was: %s\n", templates);

    printf("please input a buffer: ");
    scanf("%s", buf);
    size_t length = strlen(buf);
    if ((numWrite = write(fd, buf, length)) < length) {
        unlink(templates);
        fprintf(stderr, "write error\n");
        exit(1);
    }

    /**
     * 还未解决的问题：
     * 为什么可以使用write写入，但是不能用read读出
     * read读出是0字节
     *
     */
    if ((numRead = read(fd, str, numWrite)) < 1) {
        unlink(templates);
        fprintf(stderr, "read error\n");
        perror("");
        exit(1);
    }

    str[numRead] = '\0';
    printf("Buffer: %s\n", str);

    unlink(templates);

    if (close(fd) == -1) {
        fprintf(stderr, "close file error\n");
    }

    exit(EXIT_SUCCESS);
}
