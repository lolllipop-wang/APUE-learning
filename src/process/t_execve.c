#include <apue.h>
#include <sys/wait.h>

extern char **environ;

int main(int argc, char *argv[])
{
    if(argc < 2)
        err_sys("usage: %s progname args...");
    
    pid_t pid;

    switch ((pid = fork()))
    {
        case -1:
            err_sys("fork error");
            break;

        case 0:
            if(execve(argv[1], argv + 1, environ) == -1)
                err_sys("execve error");
        default:
            if(waitpid(pid, NULL, 0) == -1) 
                err_sys("waitpid error");
            break;
    }
    exit(0);
}