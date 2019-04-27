/**
 * 输入两个数字，输出数字之和。
 * cc -o integer_add_filter integer_add_filter.c
 */
#include <apue.h>

int main(void)
{

    int a, b;
    int nread;
    char buf[MAXLINE];

    while((nread = read(STDIN_FILENO, buf, MAXLINE)) > 0) {
        buf[nread] = 0;
        if(sscanf(buf, "%d%d", &a, &b) == 2) {
            sprintf(buf, "%d\n", a + b);

            nread = strlen(buf);

            if(write(STDOUT_FILENO, buf, nread) != nread) 
                err_sys("write error");
            
        } else {
            if(write(STDOUT_FILENO, "invalid args\n", 13) != 13) 
                err_sys("write error");
        }
    }
    
    exit(0);
}