/**
 * 使用信号同步父子进程
 * 
 * 
 */
#include <apue.h>
#include <time.h>

#define SYNCSIG SIGUSR1

int main(int argc, char *argv[])
{
    if(argc != 2)
        err_quit("usage: %s num", argv[0]);
    int i;
    pid_t pid;
    sigset_t mask;
    int communicate_nums = atoi(argv[1]);
    sigemptyset(&mask);
    sigaddset(&mask, SYNCSIG);

    if(sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
        err_sys("sigprocmask error");

    printf("SYNCSIG blocked, begin to fork\n");
    fflush(NULL);

    switch ((pid = fork()))
    {
        case -1:
            err_sys("fork error");
            break;
        case 0:
            for(i = 0; i < communicate_nums; ++i) {
                if(kill(getppid(), SYNCSIG) == -1)
                    err_sys("send sync signal to parent process failed");
                
                if(sigwaitinfo(&mask, NULL) == -1)
                    err_sys("sigwaitinfo error int child process");
                
                printf("received SYNCSIG from parent process\n");
                fflush(NULL);
            }
            exit(0);
        default:

            for(i = 0; i < communicate_nums; ++i) {

                if(sigwaitinfo(&mask, NULL) == -1)
                    err_sys("sigwaitinfo error int parent process");

                printf("received SYNCSIG from child process\n");
                fflush(NULL);

                if(kill(pid, SYNCSIG) == -1)
                    err_sys("send sync signal to child process failed");
            }
            break;
    }
    exit(0);
}