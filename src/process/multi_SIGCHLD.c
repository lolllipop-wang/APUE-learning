#include <apue.h>
#include <errno.h>
#include <sys/wait.h>

static volatile int live_children = 0;

static void handler(int sig)
{
    int status, saved_errno;
    pid_t child_pid;
    saved_errno = errno;

    printf("handler caught signal %s\n", strsignal(sig));

    while((child_pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("handler: reaped child %ld - ", (long)child_pid);
        pr_exit(status);
        live_children--;
    }

    if(child_pid == -1 && errno != ECHILD) 
        err_msg("waitpid error");
    
    //sleep(5);   如果加上 sleep捕获的信号数量会变少
    printf("handler returning\n\n\n");
    errno = saved_errno;
}

int main(int argc, char *argv[]) 
{
    if(argc < 2)
        err_quit("usage: %s sleepsec1 sleepsec2...", argv[1]);
    
    int i;
    pid_t pid;
    int sig_count;
    struct sigaction sa;
    sigset_t blocked_mask, empty_mask;

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;

    if(sigaction(SIGCHLD, &sa, NULL) == -1)
        err_sys("setting SIGCHLD handler error");
    
    sigemptyset(&blocked_mask);
    sigaddset(&blocked_mask, SIGCHLD);

    if(sigprocmask(SIG_SETMASK, &blocked_mask, NULL) == -1) 
        err_sys("block SIGCHLD error");
    
    sigemptyset(&empty_mask);

    setbuf(stdout, NULL);
    live_children = argc - 1;
    for(i = 1; i < argc; ++i) {

        switch ((pid = fork()))
        {
            case -1:
                err_sys("fork error");
                break;
            case 0:
                sleep(atoi(argv[i]));
                printf("child %ld exitting\n", (long)getpid());
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }
    sig_count = 0;
    for(;;) {
        if(sigsuspend(&empty_mask) == -1 && errno != EINTR)
            err_msg("sigsuspend error");
        sig_count++;
        if(live_children == 0)
            break;
    }

    printf("All children terminated, caughted %d signals\n", sig_count);
    exit(0);
}