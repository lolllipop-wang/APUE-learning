#define _GNU_SOURCE
#include <apue.h>
#include <setjmp.h>
#include "../../include/signal_functions.h"

static volatile sig_atomic_t can_jmp = 0; 

#ifdef SIG_SETJMP
static sigjmp_buf sig_env;
#else
static jmp_buf env;
#endif

static void handler(int sig)
{
    printf("Received signal %d(%s), sigmask is:\n", sig, strsignal(sig));

    print_sig_mask(stdout, "\t\t");

    if(can_jmp == 0) {
        printf("'env' buf not yet set, doing a simple return\n");
        return;
    }

#ifdef SIG_SETJMP
    siglongjmp(sig_env, 1);
#else
    longjmp(env, 1);
#endif

}

int main(int argc, char *argv[])
{

    print_sig_mask(stdout, "sigmask before startup:\n");

    struct sigaction action;

    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handler;

    if(sigaction(SIGINT, &action, NULL) == -1) 
        err_sys("sigaction error");
    
#ifdef SIG_SETJMP
    printf("calling sigsetjmp()\n");
    if(sigsetjmp(sig_env, 1) == 0) {
#else
    printf("calling setjmp()\n");
    if(setjmp(env) == 0) {
#endif
        can_jmp = 1;
    }

    else {
        printf("After jump back from handler, ");
        print_sig_mask(stdout, "sigmask is:\n");
    }

    for(;;)
        pause();


    exit(0);
}