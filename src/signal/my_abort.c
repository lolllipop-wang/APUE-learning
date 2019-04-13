#include <apue.h>

static volatile sig_atomic_t can_abort = 0;

static void my_abort()
{
    if(kill(getpid(), SIGABRT) == -1)
        err_sys("my_abrot error");
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