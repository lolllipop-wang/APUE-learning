#include <apue.h>
#include <errno.h>
#include <sys/wait.h>


char buf[MAXLINE];

int my_system(char *cmd)
{
    int status;
    pid_t pid, saved_pid;
    struct sigaction ign, quitact, intact;
    sigset_t new_block_mask, old_block_mask;

    /**
     * 
     * 如果cmd为NULL，并且shell可用，返回非零值。
     * 如果cmd为NULL，并且shell不可用，返回零。
     * 通过递归调用实现。
     * 
     */
    if(cmd == NULL)
        return my_system(":") == 0;

    /**
     * 阻塞SIGCHLD, SIGINT, SIGQUIT
     * 
     *    假设调用my_system()的进程还创建了其他子进程，对SIGCHLD的处
     * 理器自身也执行了wait()。此时，my_system()所创建的子进程终止，并
     * 且产生SIGCHLD信号，在my_system()调用waitpid()之前，主程序的信号
     * 处理函数可能率先得以执行（收集子进程的状态）。这是会产生竞争条件。
     * 
     * 不阻塞SIGCHLD可能会产生以下不良后果：
     * 1. 调用进程会误以为自己创建的某个子进程终止。
     * 2. my_system()函数无法获取所创建的子进程终止状态。
     */
    sigemptyset(&new_block_mask);
    sigaddset(&new_block_mask, SIGCHLD);
    sigaddset(&new_block_mask, SIGINT);
    sigaddset(&new_block_mask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &new_block_mask, &old_block_mask);

    switch ((pid = fork()))
    {
        case -1:
            sigprocmask(SIG_SETMASK, &old_block_mask, NULL);
            return -1;
        case 0:
            sigprocmask(SIG_SETMASK, &old_block_mask, NULL);
            execl("usr/bin/sh", "sh", "-c", cmd, (char *)NULL);
            _exit(127);
        default:
            break;
    }

    ign.sa_flags = 0;
    sigemptyset(&ign.sa_mask);
    ign.sa_handler = SIG_IGN;
    sigaction(SIGINT, &ign, &intact);
    sigaction(SIGQUIT, &ign, &quitact);

    saved_pid = pid;
    do {
        pid = waitpid(saved_pid, &status, 0);
    }while(pid == -1 && errno == EINTR);

    sigaction(SIGINT, &intact, NULL);
    sigaction(SIGQUIT, &quitact, NULL);
    sigprocmask(SIG_SETMASK, &old_block_mask, NULL);;
    return pid == -1 ? -1 : status;
}

int main(int argc, char *argv[]) 
{
    int status;
    for(;;) {
        printf("commamd: ");
        fflush(stdout);

        if(fgets(buf, MAXLINE, stdin) == NULL)
            break;
        
        status = my_system(buf);
        printf("system() returned: status = 0x%04x (%d, %d)\n", 
                (unsigned int)status, status >> 8, status & 0xff);
        
        if(status == -1)
            err_sys("system error");
        else {
            if(WIFEXITED(status) && WEXITSTATUS(status) == 127)
                printf("(probably)could't invoke shell\n");
            else 
                pr_exit(status);
        }
    }
    exit(0);
}