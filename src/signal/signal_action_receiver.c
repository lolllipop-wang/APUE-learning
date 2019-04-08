#define _GNU_SOURCE
#include <apue.h>
#include "../../include/signal_functions.h"

static volatile sig_atomic_t sig_int;

static int sig_cnt[NSIG];

static void 
my_signal_handler(int sig)
{
    if(sig == SIGINT) {
        sig_int = 1;
    }
    else {
        sig_cnt[sig]++;
    }
}

int
main(int argc, char *argv[])
{
    int sig, num_sec;
    sigset_t blocking_mask, pending_mask, empty_mask;
    struct sigaction action[NSIG];
    
    printf("%s: PID is %ld\n", argv[0], (long)getpid());

    
    for(sig = 1; sig < NSIG; ++sig) {
        action[sig].sa_handler = my_signal_handler;
        action[sig].sa_flags = 0;
        sigemptyset(&action[sig].sa_mask);
        sigaddset(&action[sig].sa_mask, SIGQUIT);
        if(sigaction(sig, &action[sig], NULL) == -1)
            err_sys("sigaction error");
    }
        
    
    if(argc > 1) {
        num_sec = atoi(argv[1]);

        sigfillset(&blocking_mask);

        if(sigprocmask(SIG_SETMASK, &blocking_mask, NULL) == -1)
            err_sys("sigprocmask error");
        
        printf("%s: sleeping for %d seconds\n", argv[0], num_sec);

        sleep(num_sec);

        if(sigpending(&pending_mask) == -1)
            err_sys("sigpending error");
        
        printf("%s: Pending signals are: \n");
        print_sigset(stdout, "Pending signals:\t", &pending_mask);
        
        sigemptyset(&empty_mask);

        if(sigprocmask(SIG_SETMASK, &empty_mask, NULL) == -1)
            err_sys("sigprocmask error");
    }

    while(sig_int == 0) continue;

    for(sig = 1; sig < NSIG; ++sig)
        if(sig_cnt[sig] > 0)
            printf("%s: signal %d caught %d time%s\n", argv[0], sig, sig_cnt[sig],
                    (sig_cnt[sig] == 1 ? "" : "s"));
                    
    exit(EXIT_SUCCESS);
}