/**
 * 
 * 错误示范
 * 
 */
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>

static jmp_buf env;

static void
func()
{
    if(setjmp(env) == 0) 
        printf("setjmp initialized!!!\n");
    else 
        printf("jump back\n");
}

int
main(int argc, char *argv[])
{
    func();
    /**
     * longjmp 的错误使用，函数已经返回，导致段错误。
     * [1]    7671 segmentation fault (core dumped)  ./wrong_jmp
     * 
     */
    longjmp(env, 1);
    exit(EXIT_SUCCESS);
}