/**
 * 可以实现复制标准输入到标准输出，
 * 也可以复制任意UNIX普通文件。
 * cc -o cp2 cp2.c -lapue
 * cp2 < inputfile > outputfile
 */ 
#include <apue.h>

int main()
{
    int     c;
    while((c = getc(stdin)) != EOF) {
        if(putc(c, stdout) == EOF) {
            err_sys("output error");
        }
    }

    if(ferror(stdin)) {
        err_sys("input error");
    }
    return 0;
}