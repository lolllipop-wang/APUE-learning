#define _GNU_SOURCE
#include <apue.h>
#include "../../include/signal_functions.h"

static int sig_cnt[NSIG];

/**
 * 标记是否接受到SIGINT
 */
static sig_atomic_t sig_int = 0;

static void
handler(int sig)
{
    if(sig == SIGINT) {
        sig_int = 1;
    }
    else {
        /**
         * 对接收到的信号次数进行记录
         */
        sig_cnt[sig]++;
    }
}

int
main(int argc, char *argv[])
{
    int num_sec, sig;
    sigset_t pending_mask, blocking_mask, empty_mask;

    printf("%s: PID is %ld\n", argv[0], (long)getpid());

    for(sig = 1; sig < NSIG; ++sig)
        signal(sig, handler);
    
    if(argc > 1) {
        num_sec = atoi(argv[1]);

        /**
         * 初始化一个信号集，使其包含所有信号
         */
        sigfillset(&blocking_mask);

        /**
         * 修改进程信号掩码
         */
        if(sigprocmask(SIG_BLOCK, &blocking_mask, NULL) == -1) 
            err_sys("sihprocmask error");
        
        printf("%s: sleeping for %d seconds\n", argv[0], num_sec);

        sleep(num_sec);

        /**
         * 获取处于等待状态的信号集
         */
        if(sigpending(&pending_mask) == -1)
            err_sys("sigpending error");
        
        printf("%s: pending signals are: \n", argv[0]);

        print_sigset(stdout, "pending signal:\t", &pending_mask);

        /**
         * 将进程的信号集置为空
         */
        sigemptyset(&empty_mask);

        if(sigprocmask(SIG_SETMASK, &empty_mask, NULL) == -1)
            err_sys("sigprocmask error");
        
    }

    while(sig_int == 0) continue;

    /**
     * 输出捕获到的信号及捕获到的次数
     */
    for(sig = 1; sig < NSIG; ++sig)
        if(sig_cnt[sig] > 0)
            printf("%s: signal %d caught %d time%s\n", argv[0], sig, sig_cnt[sig],
                    (sig_cnt[sig] == 1 ? "" : "s"));
        
    exit(EXIT_SUCCESS);
}