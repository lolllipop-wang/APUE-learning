/**
 *    将源文件integer_add_filter.c编译的程序integer_add_filter
 * 用作简单过滤程序，本程序从标准输入读入数据，并且将数据通过管道传送给
 * 协同进程(integer_add_filter)处理，处理后又从管道将数据读回来输出
 * 到标准输出。
 *                        example
 * ============================================================
 * +      parent                                child         +
 * +   /-------------        pipe1          --------------\   +
 * +   |  pipefd1[1]|---------------------->|stdin        |   +
 * +   |  pipefd2[0]|<----------------------|stdout       |   +
 * +   \-------------        pipe2          --------------/   +
 * ============================================================
 */
#include <apue.h>

static void handler(int sig) {
    printf("caught signal %d(%s)\n", sig, strsignal(sig)); /* unsafe */
    exit(1);
}

int main(void) {
    pid_t pid;
    int length;
    char buf[MAXLINE];
    int pipefd1[2], pipefd2[2];
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigaction(SIGPIPE, &sa, NULL) < 0)
        err_sys("setting up SIGPIPE handler error");

    if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0)
        err_sys("pipe error");

    switch ((pid = fork())) {
        case -1:
            err_sys("fork error");

        case 0:                /* child */
            close(pipefd1[1]); /* 关闭不需要的文件描述符 */
            close(pipefd2[0]);

            if (pipefd1[0] != STDIN_FILENO) { /* 将标准输入重定向到管道1的读端 */
                if (dup2(pipefd1[0], STDIN_FILENO) != STDIN_FILENO)
                    err_sys("dup2 error");
                close(pipefd1[0]);
            }

            if (pipefd2[1] != STDOUT_FILENO) { /* 将标准输出重定向到管道2的写端 */
                if (dup2(pipefd2[1], STDOUT_FILENO) != STDOUT_FILENO)
                    err_sys("dup2 error");
                close(pipefd2[1]);
            }
            /* 执行过滤程序 */
            execl("./integer_add_filter", "integer_add_filter", (char *)NULL);
            _exit(127);
        default:
            break;
    }

    close(pipefd1[0]);
    close(pipefd2[1]);

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        length = strlen(buf);

        if (write(pipefd1[1], buf, length) != length)
            err_sys("write error to pipe");

        if ((length = read(pipefd2[0], buf, MAXLINE)) < 0)
            err_sys("read from pipe error");

        if (length == 0)
            err_sys("child closed pipe");

        buf[length] = 0;

        if (fputs(buf, stdout) == EOF)
            err_sys("fputs yo stdout error");
    }

    if (ferror(stdin))
        err_sys("fgets from stdin error");

    exit(0);
}