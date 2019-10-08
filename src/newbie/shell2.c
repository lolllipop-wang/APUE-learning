/**
 * UNIX中的进程控制功能可以使用这个程序说明。
 * 它从标准输出读取命令，然后执行命令，类似于一个简单的shell。
 * 这个简易版shell不能向所执行的命令传递参数。
 * cc -o shell shell.c -lapue
 * ./shell
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 4096

/**
 * 以下为新添加的程序
 *
 * d当输入Ctrl + C中断程序时，产生了指定的SIGINT信号，sig_int函数
 * 将会被调用，并打印一条信息。
 */
static void sig_int(int);

int main() {
    char buf[MAXLINE]; /* MAXLINE在apue.h中定义，限制一行输入的最大字符数 */
    pid_t pid;
    int status;

		if (signal(SIGINT, sig_int) == SIG_ERR) {
				fprintf(stderr, "signal error\n");
				exit(1);
		}
        

    printf("<[shell]>%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        /**
         * 1. fgets从标准输入每次读取一行，输入文件结束符（Ctrl +
         * D）时返回NULL，循环停止。
         * 2.
         * fgets读入的每一行都以换行符结束，但是execlp函数要求参数以NUL结束，而不是换行符，
         * 因此使用0替换'\n'。
         */
        size_t length = strlen(buf);
        if (buf[length - 1] == '\n')
            buf[length - 1] = 0;

        if (strcmp(buf, "exit") == 0) { /* 输入exit命令时结束程序，退出shell */
            printf("Bye bye!\n");
            return 0;
        }

        /**
         * 3. 使用fork创建一个新进程，
         * 新进程是调用进程的一个副本，称调用进程为父进程，新进程为子进程，
         * fork函数对父进程返回子进程的PID，
         * 对子进程返回0，因此fork函数被父进程调用一次，返回两次。
         * 子进程会执行fork函数后面的语句，而且会复制父进程的缓冲区。
         */
        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error\n");
						exit(1);
        }
        /**
         * 4.
         * 在子进程中使用execlp执行输入的命令的程序文件，这就用新程序文件替换了原先的程序文件。
         * 某些操作系统使用fork和exec组合产生一个新进程。
         * 5. 子进程调用execlp执行程序文件时，
         * 父进程通过waitpid等待子进程终止，waitpid的参数需要子进程ID，
         * 返回子进程的终止状态(status)。
         */
        else if (pid == 0) {
            execlp(buf, buf, (char *)0);
            fprintf(stderr, "couldn't execute: %s\n", buf);
            exit(127);
        }

        if ((pid = waitpid(pid, &status, 0)) < 0) {
            fprintf(stderr, "waitpid error\n");
						exit(1);
        }
        printf("<[shell]>%% ");
    }
    printf("\n");
    return 0;
}

void sig_int(int signo) { printf("interrupt\n%% "); }
