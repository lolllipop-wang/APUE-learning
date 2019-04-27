/**
 * 
 * 使用管道同步父子进程
 */
#ifndef SYNC_PIPE_H
#define SYNC_PIPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <apue.h>

static int pipefd1[2], pipefd2[2];

/**
 * 初始化 
 */
void TELL_WAIT(void)
{
    if(pipe(pipefd1) < 0 || pipe(pipefd2) < 0)
        err_sys("%s: pipe error", "TELL_WAIT");
}

/**
 * 子进程向父进程通知，向子进程管道写端写入一个字符'c'
 */
void TELL_PARENT(/*pid_t ppid*/)
{
    if(write(pipefd2[1], "c", 1))
        err_sys("%s: write error", "TELL_PARENT");
}

/**
 * 子进程等待父进程，期望从父进程管道读端读入一个'p'字符
 */
void WAIT_PARENT(void)
{
    char c;
    if(read(pipefd1[0], &c, 1) != 1)
        err_sys("%s: read error", "WAIT_PARENT");
    
    if(c != 'p')
        err_quit("%s: incorrect data", "WAIT_PARENT");

}

/**
 * 父进程通知子进程，向父进程管道写端写入一个字符'p'
 */
void TELL_CHILD(/*pid_t pid*/)
{
    if(write(pipefd1[1], "p", 1) != 1)
        err_sys("%s: write error", "TELL_CHILD");
}

/**
 * 附近成等待子进程，期望从子进程管道读端读入一个字符'c'
 */
void WAIT_CHILD(void)
{
    char c;
    if(read(pipefd2[0], &c, 1) != 1)
        err_sys("%s: read error", "WAIT_CHILD");
    
    if(c != 'c')
        err_quit("%s: incorrect data", "WAIT_CHILD");
}


#ifdef __cplusplus
}
#endif

#endif  /* SYNC_PIPE_H */