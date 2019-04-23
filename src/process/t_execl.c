/**
 * 使用fork()和exec函数运行另一个程序
 */
#include <apue.h>
#include <sys/wait.h>

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
            status = execl("/usr/bin/uname", "uname", "-a", (char *)0);
            if(status < 0)
                err_sys("execl failed");
            
        default:

            if(waitpid(pid, &status, 0) < 0)
                err_sys("waitpid error");
            
            pr_exit(status);
            break;
    }
    exit(0);
}