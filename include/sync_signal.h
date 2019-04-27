#ifndef SYNC_SIGNAL_H
#define SYNC_SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <apue.h>

static sig_atomic_t sync_flag = 0;         /* 同步标志 */
static sig_atomic_t from_parent = 0;       /* 同步信号来自父进程 */
static sig_atomic_t from_child = 0;        /* 同步信号来自子进程 */
static struct sigaction sa;

static sig_atomic_t PARENT_SIG = SIGUSR1;   /* 发送给子进程的信号 */
static sig_atomic_t CHILD_SIG = SIGUSR2;    /* 发送给父进程的信号 */

static sigset_t oldmask, newmask, emptymask;

static void sync_sig_handler(int sig)       /* 信号处理函数 */
{
    sync_flag = 1;                          /* 接收到同步信号 */
    sig == PARENT_SIG ? from_parent = 1 : 
                        from_child  = 1;
}

/**
 * 初始化 
 */
void TELL_WAIT(void)
{
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sync_sig_handler;

    if(sigaction(PARENT_SIG, &sa, NULL) < 0)    /* 为PARENT_SIG设置信号处理函数 */
        err_sys("TELL_WAIT: setting"
                "PARENT_SIG handler error");
    
    if(sigaction(CHILD_SIG, &sa, NULL) < 0)     /* 为CHILD_SIG设置信号处理函数 */
        err_sys("TELL_WAIT: setting"
                "CHILD_SIG handler error");
    
    sigemptyset(&emptymask);
    sigemptyset(&oldmask);
    sigemptyset(&newmask);
    sigaddset(&newmask, PARENT_SIG);
    sigaddset(&newmask, CHILD_SIG);

    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)  /* 阻塞PARENT_SIG和CHILD_SIG */
        err_sys("TELL_WAIT: block PARENT_SIG and"
                 "CHILD_SIG error");
}

/**
 * 子进程向父进程发送信号CHILD_SIG
 */
void TELL_PARENT(pid_t ppid)
{
    kill(ppid, CHILD_SIG);
}

/**
 * 子进程等待父进程，期望接收到PARENT_SIG
 */
void WAIT_PARENT(void)
{
    while(sync_flag == 0 && from_parent == 0)
        sigsuspend(&emptymask);
    
    sync_flag = 0;
    from_parent = 0;
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("TELL_PARENT: reset old mask error");
}

/**
 * 父进程向子进程发送信号PARENT_SIG
 */
void TELL_CHILD(pid_t pid)
{
    kill(pid, PARENT_SIG);
}

/**
 * 父进程等待子进程，期望接收到CHILD_SIG
 */
void WAIT_CHILD(void)
{
    while(sync_flag == 0 && from_child == 0)
        sigsuspend(&emptymask);
    
    sync_flag = 0;
    from_child = 0;
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("TELL_PARENT: reset old mask error");
}

#ifdef __cplusplus
}
#endif

#endif  /* SYNC_SIGNAL_H */