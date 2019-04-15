#include <apue.h>

static volatile sig_atomic_t can_abort = 0;

static void my_abort()
{
    struct sigaction sa;

    sigfillset(&sa.sa_mask);
    sigdelset(&sa.sa_mask, SIGABRT);
    sigprocmask(SIG_SETMASK, &sa, NULL);
    raise(SIGABRT);


    sa.sa_handler = SIG_DFL;
    sa.sa_flags = 0;
    sigfillset(&sa.sa_mask);
    sigdelset(&sa.sa_mask, SIGABRT);
    sigprocmask(SIG_SETMASK, &sa, NULL);
    raise(SIGABRT);
    exit(1);
}

static void handler(int sig)
{
    printf("Caught a signal %d(%s)\n", sig, strsignal(sig));
    can_abort = 1;
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGINT, &sa, NULL) == -1)
        err_sys("sigaction error");
    
    printf("Ctrl C to abort\n");
    for(;;) {
        if(can_abort == 1)
            my_abort();
    }

    exit(0);
}