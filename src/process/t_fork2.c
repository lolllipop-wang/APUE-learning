/**
 * 使用fork()创建的子进程对变脸的更改并不影响父进程。
 * 使用vfork()创建的子进程对变脸的更改影响父进程。
 */
#include <apue.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int globvar = 6;
char buf[] = "write to stdout\n";

int main()
{
    int var;
    pid_t pid;

    var = 88;

    if(write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
        err_sys("write error");

#ifdef VFORKMODE
    printf("before vfork\n");
#else 
    printf("before fork\n");
#endif
    fflush(stdout);

#ifdef VFORKMODE
    switch ((pid = vfork()))
#else
    switch ((pid = fork()))
#endif
    {
        case -1: 
#ifdef VFORKMODE
            err_sys("vfork error");
#else
            err_sys("fork error");
#endif
        case 0:
            globvar++;
            var++;
            break;
    
        default:
            waitpid(pid, NULL, 0);
            break;
    }

    printf("pid = %ld, globvar = %d, var = %d\n", (long)pid, globvar, var);
    exit(0);
}