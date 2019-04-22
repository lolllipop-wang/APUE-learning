/**
 * #include <sys/wait.h>
 * waitid(idyype idtype, id_t id, siginfo_t *infop, int options);
 */
#include <apue.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    siginfo_t info;

    switch ((pid = fork()))
    {
        case -1:                    /* failed to create child process */
            err_sys("fork error");
            break;
        case 0:                     /* child */
            printf("I'am child\n");
            if(kill(getpid(), SIGABRT) < 0)
                err_sys("kill self error");
        default:                    /* parent */
            if(waitid(P_PID, pid, &info, WEXITED) == -1)
                err_sys("waitid error");
            
            printf("signal: %d(%s)\n", info.si_signo, strsignal(info.si_signo));
            break;
    }

    exit(0);
}