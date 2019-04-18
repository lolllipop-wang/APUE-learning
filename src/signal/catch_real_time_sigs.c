#define _GNU_SOURCE
#include <apue.h>

static volatile int sig_cnt;
static volatile sig_atomic_t all_done;
static volatile int handler_sleep_time = 1;


static void handler(int sig, siginfo_t *info, void *ucontext)
{
    if(sig == SIGINT || sig == SIGTERM) {
        all_done = 1;
        return;
    }

    sig_cnt++;
    char *sig_code_buf = info -> si_code == SI_USER ? "SI_USER" : 
                         info -> si_code == SI_QUEUE ? "SI_QUEUE" : "other"; 

    printf("caught signal %d\n", sig);
    printf("\tsi_signo = %d(%s), si_code = %d(%s), si_value = %d\n",
            info -> si_signo, strsignal(info -> si_signo), info -> si_code, 
            sig_code_buf, info -> si_value.sival_int);
    printf("\tPID = %ld, UID = %ld\n", (long)info -> si_pid, (long)info -> si_uid);
    sleep(handler_sleep_time);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        err_quit("usage: %s block-time");
    int sig;
    struct sigaction action;
    sigset_t blocking_mask, old_mask;

    printf("%s: PID = %ld\n", argv[0], (long)getpid());

    action.sa_handler = handler;
    action.sa_flags = SA_SIGINFO;
    sigfillset(&action.sa_mask);

    for(sig = 1; sig < NSIG; ++sig) {
        if(sig == SIGKILL || sig == SIGSTOP || sig == 32 || sig == 33)
            continue;
        if(sigaction(sig, &action, NULL) == -1)
            err_sys("sigaction error");
    }
    
    sigfillset(&blocking_mask);
    sigdelset(&blocking_mask, SIGQUIT);
    sigdelset(&blocking_mask, SIGSTOP);

    if(sigprocmask(SIG_SETMASK, &blocking_mask, &old_mask) < 0)
        err_sys("sigprocmask error");
    
    printf("%s: signal blocking, sleep %s seconds\n", argv[0], argv[1]);
    sleep(atoi(argv[1]));
    printf("%s: sleep complete\n", argv[0]);

    if(sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
        err_sys("sigprocmask error");
    
    while(all_done == 0)
        pause();


    exit(0);
}

