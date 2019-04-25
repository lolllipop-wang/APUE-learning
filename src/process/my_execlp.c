
#define _DEFAULT_SOURCE
#include <apue.h>
#include <paths.h>
#include <errno.h>
#include <alloca.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/param.h>


extern char **environ;

static int my_execlp(char *name, const char *arg, ...)
{
    va_list ap;                         /* 获取可变长参数 */
    int argc, envc;                     /* 参数和环境变量的数量 */
    int cnt, lp, ln, n;                 /* lp表示路径名的长度，ln表示文件名的长度 */       
    char **argv;                        /* argv */
    char **envp;                        /* envp */
    int eacces, saved_errno;            /* eacces标记是否是无权访问引发的错误*/
    char *cur, buf[MAXPATHLEN];         /* buf存储路径名+文件名产生的字符串 */
    
    const char *p, *bp, *path, **memp;  /* path指向PATH变量， memp指向传递给shell的参数*/
    struct stat sb;                     /* 在判断EACCES原因时使用 */

    /* 开始构建argv */
    va_start(ap, arg);
    n = 1;
    while(va_arg(ap, char *) != NULL)               /* 获取arg的数量 */
        argc++;
    
    argv = alloca((argc + 1) * sizeof(char *));

    if(argv != NULL) {
        errno = ENOMEM;
        return -1;
    }

    n = 1;
    va_start(ap, arg);
    argv[0] = (char *)arg;
    while((argv[n] = va_arg(ap, char *)) != NULL)   
        n++;
    va_end(ap);
    

    /* 开始构建envp */
    for(envc = 0; environ[envc] != NULL; envc++)
        ;

    envp = alloca(((envc + 1) * sizeof(char *)));
    
    for(envc = 0; environ[envc] != NULL; envc++) 
        envp[envc] = environ[envc];
    
    envp[envc] = NULL;

    /* 获取PATH变量 */

    path = getenv("PATH");
    if(path == NULL)
        path = _PATH_DEFPATH;

    cur = alloca(strlen(path) + 1);
    if(cur == NULL) {
        errno = ENOMEM;
        return -1;
    }
    /* 开始尝试执行execve */

    /* 如果是一个绝对路径 */
    if(strchr(name, '/') != NULL) {
        bp = name;
        cur = NULL;
        goto retry;     /* 直接跳转到retry去执行execve */
    }
    bp = buf;

    if(*name == '\0') {
        errno = ENOENT;
        return -1;
    }

    strcpy(cur, path);

    /* 分解PATH变量 */
    while ((p = strsep(&cur, ":")) != NULL) {

        if(*p == '\0') {
            p = ".";
            lp = 1;
        } else 
            lp = strlen(p);  /* 从PATH变量中分离出来的某一个路径的长度 */
        ln = strlen(name);   /* 可执行文件的名称 */

        if(lp + ln + 2 > MAXPATHLEN) {  /* 路径长度+文件名长度大于最大限制长度 */
            err_msg("path too lang");
            continue;
        } 

        /* 将路径名和文件名拼接成一个完整的绝对路径 */
        bcopy(p, buf, lp);
        buf[lp] = '/';
        bcopy(name, buf + lp + 1, ln);
        buf[lp + 1 + ln] = '\0';

retry:  execve(p, argv, envp);

        switch (errno)
        {
            case E2BIG:         /* 参数长度过长 */
                //TODO goto done;
                break;           
            case ELOOP:         /* 符号连接层数过多 */
            case ENAMETOOLONG:  /* 文件名过长 */
            case ENOENT:        /* 找不到文件或目录 */
                break;
            case ENOEXEC:       /* 可执行文件格式错误 */

                /* 可能是一个脚本文件，尝试用shell执行 */
                for(cnt = 0; argv[cnt]!= NULL; ++cnt)
                    ;
                memp = alloca((cnt + 2) * sizeof(char *));

                if(memp = NULL) {
                    errno = ENOMEM;
                    //TODO goto done;
                }
                memp[0] = "sh";
                memp[1] = bp;

                bcopy(argv + 1, memp + 2, cnt * sizeof(char *));
                execve(_PATH_BSHELL, (char * const *)memp, (char * const *)envp);
                goto done;
            case ENOMEM:
                goto done;

            case ENOTDIR:           /* 不是一个目录 */
                goto done;

            case ETXTBSY:           /* 文本文件正在被占用 */
                goto done;

            default:
                /**
                 * 
                 * EACCES可能是因为没有文件夹访的问权限或者
                 * 没有执行权限，使用stat来尝试获取文件信息，
                 * 进而确定引发错误的原因是上面所说的两种中
                 * 的哪一种。
                */
                saved_errno = errno;
                if(stat(bp, &sb) != 0)
                    break;
                
                /* 没有访问权限 */
                if(saved_errno == EACCES) {
                    eacces = 1;
                    continue;
                }
                errno = saved_errno;
                goto done;
                break;
        }
    }

    if(eacces == 1)
        errno = EACCES;
        else errno = ENOENT;

done:   /* 函数执行失败 */
    return -1;
}

int main(int argc, char *argv[])
{
    char    buf[MAXLINE]; /* MAXLINE在apue.h中定义，限制一行输入的最大字符数 */
    pid_t   pid;
    int     status;

    printf("<[shell]>%% ");
    while(fgets(buf, MAXLINE, stdin) != NULL) {
        
        int length = strlen(buf);
        if(buf[length - 1] == '\n')
            buf[length - 1] = 0;

        if(strcmp(buf, "exit") == 0) { /* 输入exit命令时结束程序，退出shell */
            printf("Bye bye!\n");
            return 0;
        }
       
        if((pid = fork()) < 0) {
            err_sys("fork error");
        }
        
        else if(pid == 0) {
            execlp(buf, buf, (char *)0);
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        if((pid = waitpid(pid, &status, 0)) < 0) {
            err_sys("waitpid error");
        }
        printf("<[shell]>%% ");
    }
    printf("\n");
    exit(0);
}