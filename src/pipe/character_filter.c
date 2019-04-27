/**
 * 将输入的字符中的小写字符转换为大写，丢弃除数字和字母之外的特殊字符。
 * cc -o character_filter character_filter.c -lapue
 */
#include <apue.h>
#include <ctype.h>

#define NOFILTER(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '\n')

int main(int argc, char *argv[])
{
    int c;
    while((c = getchar()) != EOF) {
        if(!NOFILTER(c))
            continue;
        if(!isupper(c))
            c = toupper(c);
        if(putchar(c) == EOF)
            err_sys("%s: output error", argv[0]);
        if(c == '\n')
            fflush(stdout);
    }
    exit(0);
}