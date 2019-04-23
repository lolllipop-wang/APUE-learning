/**
 * ===========begin============
 * ./a.out > temp
 * cat temp
 * This is a string form write.
 * This is a string from printf.
 * This is a string from printf.
 * ============end==============
 * 由于fork会复制父进程的堆栈，因此也会
 * 复制stdio的缓冲区，在进程结束前调用
 * exit()时会刷新缓冲区，因此printf()
 * 输出的字符串就会被输出两次。
 */
#include <apue.h>

int main()
{
    pid_t pid;
    char *buf = "This is a string form write.\n";
    write(STDOUT_FILENO, buf, strlen(buf));
    printf("This is a string from printf.\n");      
    switch ((pid = fork()))
    {
        case -1:
            err_sys("fork error");
            break;
        case 0:
            exit(0);

        default:
            break;
    }
    exit(0);
}