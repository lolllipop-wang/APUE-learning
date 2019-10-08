/**
 * 可以用于复制任何一个UNIX普通文件
 * 使用shell重定向输入和输出。
 * cc -o cp ./cp.c -lapue
 * ./cp < inputfile > outputfile
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 4096

int main() {
    size_t n;
    char buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            fprintf(stderr, "write error\n");
        }
    }

    if (n < 0) {
        fprintf(stderr, "read error\n");
        exit(1);
    }
    return 0;
}
