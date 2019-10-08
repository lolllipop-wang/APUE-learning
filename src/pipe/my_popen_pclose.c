/**
 * 实现popen和pclose
 */
#include <apue.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

#define OPEN_MAX_GUESS 256

static pid_t *childpid = NULL; /* 保存子进程PID */
static int max_fd;             /* 打开文件的最大个数的近似值 */

FILE *my_popen(const char *cmd, char *type) {
    FILE *fp;
    pid_t pid;
    int i, pipefd[2];

    if (cmd == NULL || type == NULL) {
        errno = EINVAL;
        return NULL;
    }

    /* 检查type是否合法 */
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
        errno = EINVAL;
        return NULL;
    }

    if (childpid == NULL) { /* 第一次调用my_popen时为childpid申请空间 */
        max_fd = open_max();

        childpid = calloc(max_fd, sizeof(pid_t));

        if (childpid == NULL)
            return NULL;
    }

    if (pipe(pipefd) < 0) /* 建立管道 */
        return NULL;

    /* 打开文件过多 */
    if (pipefd[0] >= max_fd || pipefd[1] >= max_fd) {
        close(pipefd[0]);
        close(pipefd[1]);
        errno = EMFILE;
        return NULL;
    }

    switch ((pid = fork())) {
        case -1:
            return NULL;
        case 0:                   /* child */
            if (type[0] == 'r') { /* 关闭管道读端，并将管道写端重定向到标准输出 */
                close(pipefd[0]);
                if (pipefd[1] != STDOUT_FILENO) {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                }
            } else { /* 关闭管道写端，并将管道读端重定向到标准输入 */
                close(pipefd[1]);
                if (pipefd[0] != STDIN_FILENO) {
                    dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[0]);
                }
            }

            for (i = 0; i < max_fd; ++i) /* 关闭childpis[]中的所以文件描述符 */
                if (childpid[i] > 0)
                    close(i);

            execl("/usr/bin/sh", "sh", "-c", cmd, (char *)NULL); /* 用shell执行cmd */
            _exit(127);

        default:
            break;
    }

    /* parent */
    if (type[0] == 'r') { /* 关闭管道写端，并且获得管道读端对应的文件描述符 */
        close(pipefd[1]);
        fp = fdopen(pipefd[0], type);
        if (fp == NULL)
            return NULL;
    } else { /* 关闭管道读端，并且获得管道写端对应的文件描述符 */
        close(pipefd[0]);
        fp = fdopen(pipefd[1], type);
        if (fp == NULL)
            return NULL;
    }

    childpid[fileno(fp)] = pid; /* 记录这个当前文件描述符对应的子进程PID */
    return fp;
}

int my_pclose(FILE *fp) {
    pid_t pid;
    int fd, status;

    if (fp == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (childpid == NULL) {
        errno = EINVAL;
        return -1;
    }

    fd = fileno(fp);
    if (fd > max_fd) {
        errno = EINVAL;
        return -1;
    }

    pid = childpid[fd];

    if (pid == 0) {
        errno = EINVAL;
        return -1;
    }

    childpid[fd] = 0;
    if (fclose(fp) == EOF)
        return -1;

    while (waitpid(pid, &status, 0) < 0)
        if (errno = EINTR)
            return -1;

    return status;
}

/* 直接使用my_pager_popen.c中的代码 */
#define PAGER                                                                                                          \
    "${PAGER:-more}" /* 如果环境变量里有PAGER,且其值非空，则使用其值，                             \
                        否则用more */

int main(int argc, char *argv[]) {
    char buf[MAXLINE];
    FILE *fin, *fout;

    if (argc != 2)
        err_quit("usage: %s file", argv[0]);

    fin = fopen(argv[1], "r");
    if (fin == NULL)
        err_sys("%s: can't open file %s", argv[0], argv[1]);

    fout = my_popen(PAGER, "w"); /* sh -c PAGER */
    if (fout == NULL)
        err_sys("%s: popen error", argv[0]);

    while (fgets(buf, MAXLINE, fin) != NULL) {
        if (fputs(buf, fout) == EOF)
            err_sys("%s: fputs error", argv[0]);
    }

    if (ferror(fin))
        err_sys("%s: fgets error", argv[0]);

    if (my_pclose(fout) == -1)
        err_sys("%s: pclose error", argv[0]);
    exit(0);
}