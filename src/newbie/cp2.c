/**
 * 可以实现复制标准输入到标准输出，
 * 也可以复制任意UNIX普通文件。
 * cc -o cp2 cp2.c -lapue
 * cp2 < inputfile > outputfile
 */
#include <stdio.h>
#include <stdlib.h>

int main() {
    int c;
    while ((c = getc(stdin)) != EOF) {
        if (putc(c, stdout) == EOF) {
            fprintf(stderr, "output error\n");
            exit(1);
        }
    }

    if (ferror(stdin)) {
        fprintf(stderr, "input error\n");
        exit(1);
    }
    return 0;
}
