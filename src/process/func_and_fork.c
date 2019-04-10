#include <apue.h>
#include <sys/wait.h>

int func()
{
    printf("function in child: %ld\n", (long)getpid());
    return 0;
}

int 
main()
{
    pid_t pid;
    int status;
    switch (pid = fork())
    {
        case -1:
            err_sys("fork error");
            break;
        case 0:
            exit(func());  
        default:
            if((pid = waitpid(pid, &status, 0)) < 0)
                err_sys("waitpid error");
            printf("child has exit %s\n", 
                    (status == EXIT_SUCCESS ? "successed" : "failed"));
            break;
    }
    exit(0);
}