/**
 * 使用宏打印进程终止状态
 */
#include <apue.h>
#include <sys/wait.h>

void print_exit_status(int status);

int main()
{
    pid_t pid;
    int status;

    switch ((pid = fork()))
    {
        case -1:
            err_sys("fork error");
            break;
        case 0:
            exit(7);
        default:
            if(wait(&status) != pid)
                err_sys("wait error");
            print_exit_status(status);
    }

    switch ((pid = fork()))
    {
        case -1:
            err_sys("fork error");
            break;
        case 0:
            abort();
        default:
            if(wait(&status) != pid)
                err_sys("wait error");
            print_exit_status(status);
    }

    switch ((pid = fork()))
    {
        case -1:
            err_sys("fork error");
            break;
        case 0:
            status /= 0;
        default:
            if(wait(&status) != pid)
                err_sys("wait error");
            print_exit_status(status);
    }
    exit(0);
}



void print_exit_status(int status)
{
    if(WIFEXITED(status))
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    else if(WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d(%s)%s\n", 
            WTERMSIG(status), strsignal(WTERMSIG(status)),
#ifdef WCOREDUMP
            WCOREDUMP(status) ? "core file generated" : "");
#else
            "");
#endif
    else if(WIFSTOPPED(status))
        printf("child stoped, signal number = %d(%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status)));
}