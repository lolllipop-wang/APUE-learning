#define _GNU_SOURCE
#include <apue.h>
#include "../../include/signal_functions.h"

static void
my_signal_handler(int sig)
{
    psignal(sig, "caught a signal");
    exit(EXIT_SUCCESS);
}

int
main(int argc ,char *argv[])
{
    int num_sec = 5;
    sigset_t blocking_mask, pending_mask;
    struct sigaction action;

    printf("%s: My PID is %ld\n", argv[0], (long)getpid());



    printf("setting up signal handler for SIGINT\n");

    /**
     * 1. setting up signal hander for SIGINT
     */
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    action.sa_handler = my_signal_handler;

    if(sigaction(SIGINT, &action, NULL) == -1)
        err_sys("sigaction error");
    
    sigemptyset(&blocking_mask);
    sigaddset(&blocking_mask, SIGINT);

    if(sigprocmask(SIG_SETMASK, &blocking_mask, NULL) == -1)
        err_sys("sigprocmask error");
    
    printf("blocking SIGINT %d seconds\n", num_sec);

    sleep(num_sec);

    if(sigpending(&pending_mask) == -1)
        err_sys("sigpending error");
    
    printf("%s: pending signals are:\n", argv[0]);
    print_sigset(stdout, "\t\t", &pending_mask);
    
    /**
     * 2. ignore SIGINT
     */
    
    printf("\n\n\nignore SIGINT begin\n");
    sleep(num_sec);
    if(signal(SIGINT, SIG_IGN) == SIG_ERR)
        err_sys("signal error");

    if(sigpending(&pending_mask) == -1)
        err_sys("sigpending error");
    
    if(sigismember(&pending_mask, SIGINT))
        printf("SIGINT is pending now\n");
    else {
        printf("%s: pending signals are:\n", argv[0]);
        print_sigset(stdout, "\t\t", &pending_mask);
    }
    printf("ignore SIGINT end\n\n\n");
    /**
     * 3. Reestablishing handler for SIGINT
     */
    printf("Reestablishing handler for SIGINT\n");

    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    action.sa_handler = my_signal_handler;

    if(sigaction(SIGINT, &action, NULL) == -1)
        err_sys("sigaction error");
    
    
    printf("blocking SIGINT %d seconds\n", num_sec);
    sleep(num_sec);
    if(sigpending(&pending_mask) == -1)
        err_sys("sigpending error");
    
    printf("%s: pending signals are:\n", argv[0]);
    print_sigset(stdout, "\t\t", &pending_mask);

    /**
     * 4. unblocking SIGINT
     */
    printf("unblocking SIGINT\n");
    sigemptyset(&blocking_mask);
    if(sigprocmask(SIG_SETMASK, &blocking_mask, NULL) == -1)
        err_sys("unblocking SIGINT error");

    exit(EXIT_SUCCESS);
}