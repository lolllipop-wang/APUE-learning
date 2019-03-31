/**
 * 打印当前程序的PID
 * cc -o printPID ./printPID.c -lapue
 */
#include <apue.h>

int main()
{
    pid_t pid;
    pid = getpid();
    printf("Hello world from process ID %ld\n", (long)pid);
    return 0;
}