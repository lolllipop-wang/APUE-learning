/**
 * 输入两个数字，输出数字之和。
 * cc -o integer_add_filter integer_add_filter.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

int main(void) {

    int a, b;
    size_t nread;
    char buf[MAXLINE];

    while ((nread = read(STDIN_FILENO, buf, MAXLINE)) > 0) {
        buf[nread] = 0;
        if (sscanf(buf, "%d%d", &a, &b) == 2) {
            sprintf(buf, "%d\n", a + b);

            nread = strlen(buf);

            if (write(STDOUT_FILENO, buf, nread) != nread) {
                fprintf(stderr, "write error\n");
                exit(1);
            }

        } else {
            if (write(STDOUT_FILENO, "invalid args\n", 13) != 13) {
                fprintf(stderr, "write error\n");
                exit(1);
            }
        }
    }

    exit(0);
}
