/**
 * 使用两次fork()调用产生僵尸进程
 */
#include <apue.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    switch ((pid = fork()))
    {
        case -1:                    /* failed to create first child */
            err_sys("fork error");
            break;
        case 0:                     /* first child */
            switch ((pid = fork()))
            {
                case -1:            /* failed to create second child */
                    err_sys("fork error");
                    break;
                case 0:             /* second child */

                    /**
                     * The second child's parent become init(or systemed) soon,
                     * 
                     */
                    sleep(2);
                    printf("second child, parent pid = %ld\n", (long)getppid());
                    exit(0);
                default:
                    printf("I'm first child, my PID = %ld\n", getpid());
                    exit(0);
            }
        default:
            if(waitpid(pid, NULL, 0) != pid)
                err_sys("waitpid error");
            break;
    }
    exit(0);
}