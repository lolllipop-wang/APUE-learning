/**
 * 打开系统进程记账功能。
 * ====================================
 * #define _BSD_SOURCE
 * #include <unistd.h>
 * 
 * int acct(const char *filename);
 * 
 * Return 0 on success, or -1 on error.
 * ====================================
 * $ su
 * Password:
 * $ touch file
 * ./progname file
 * Process accounting Enable
 * $
 * Enter Ctrl D to exit.
 */
#define _DEFAULT_SOURCE
#include <apue.h>

int main(int argc, char *argv[])
{  
    if(argc != 2)
        err_quit("usage: %s [file]", argv[0]);
    
    if(acct(argv[1]) == -1) 
        err_sys("acct error");
    
    printf("Process accounting %s\n", 
            argv[1] == NULL ? "Disable" : "Enable");
    exit(0);
}
