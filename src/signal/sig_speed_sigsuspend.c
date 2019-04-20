/**
 * 验证sigwaitinfo比sigsuspend快。
 */
#include <apue.h>
#include <errno.h>

static void handler(int sig)
{

}

int main(int argc, char *argv[])
{
    pid_t pid;
    int num_sigs = 1000000, i;
    sigset_t blocked_mask, old_mask, empty_mask;
    struct  sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if(sigaction(SIGUSR1, &sa, NULL) < 0)
        err_sys("sigaction error\n");
    
    sigemptyset(&blocked_mask);
    sigaddset(&blocked_mask, SIGUSR1);

    sigemptyset(&empty_mask);
    if(sigprocmask(SIG_SETMASK, &blocked_mask, NULL) < 0)
        err_sys("sigprocmask error");
    
    switch((pid = fork())) {
        case -1:
            err_sys("fork error");
        case 0: /* child */
            for(i = 0; i < num_sigs; ++i) {
                if(kill(getppid(), SIGUSR1) < 0)
                    err_sys("send signal to parent error");
                
                if(sigsuspend(&empty_mask) < 0 && errno != EINTR) 
                    err_sys("sigsuspend error in child process");
            }
            exit(0);
        default:
            for(i = 0; i < num_sigs; ++i) {
                if(sigsuspend(&empty_mask) < 0 && errno != EINTR)
                    err_sys("sigsuspend error in parent process");
                
                if(kill(pid, SIGUSR1) < 0)
                    err_sys("send signal to child error");
            }
            exit(0);
    }

    exit(0);
}