#define _GNU_SOURCE
#include <apue.h>
#include <setjmp.h>
#include "../../include/signal_functions.h"

static volatile sig_atomic_t can_jmp = 0;

static sigjmp_buf env;

static void handler(int sig)
{
    printf("Received signal %d(%s), sigmask is:\n", sig,strsignal(sig));
    print_sig_mask(stdout, "\t\t");

    if(can_jmp == 0) {
        printf("'env' buf not yet set, doing a simple return\n");
        return;
    }

    printf("You can't abort me!!!!!\n");

    siglongjmp(env, 1);
}

int main(int argc, char *argv[])
{

    printf("My PID is %ld\n", (long)getpid());

    struct sigaction sa;

    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGABRT, &sa, NULL) == -1)
        err_sys("sigaction error");
    
    printf("calling sigsetjmp()\n");

    if(sigsetjmp(env, 1) == 0)
        can_jmp = 1;
    else {
        printf("After jump back from handler, ");
        print_sig_mask(stdout, "sigmask is:\n");
    }

    for(;;)
        pause();
    exit(0);
}