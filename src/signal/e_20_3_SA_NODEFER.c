/**
 * 验证了SA_NODEFER标志的效果。
 * 
 *    运行信号处理函数时，内核将阻塞给定信号，但是设置SA_NODEFER后就不会阻塞，
 * 第一次运行信号处理函数时，sleep 10 sec，由于设置了SA_RESETHAND，会重置信号处理函数
 * 为SIG_DFL，此时按Ctrl + C，SIGINT信号会终止进程。
 *
 */
#define _GNU_SOURCE
#include <apue.h>
#include "../../include/signal_functions.h"

static volatile sig_atomic_t sig_int = 0;

static void
my_signal_handler(int sig)
{
    if(sig == SIGINT) {
        sig_int = 1;
        psignal(sig, "caught a signal when blocking");
        sleep(5);
    }
}

int
main(int argc, char *argv[])
{   
    int num_sec = 10;
    sigset_t blocking_mask;
    sigset_t pending_mask;
    struct sigaction action;
    action.sa_flags = SA_RESETHAND | SA_NODEFER;
    action.sa_handler = my_signal_handler;

    if(sigaction(SIGINT, &action, NULL) == -1)
        err_sys("sigaction error");
    
    printf("setting up for SIGINT\n");
    sigemptyset(&blocking_mask);
    sigaddset(&blocking_mask, SIGINT);

    //if(sigprocmask(SIG_SETMASK, &blocking_mask, NULL) == -1)
        //err_sys("sigprocmast error");
    
    printf("first waitting fro SIGINT\n");
    while(sig_int == 0) continue;
    /*
    if(sigpending(&pending_mask) == -1)
        err_sys("sigpending error");
    printf("pending signals are:\n");
    print_sigset(stdout, "\t\t", &pending_mask);
    */
    
    exit(EXIT_SUCCESS);
}