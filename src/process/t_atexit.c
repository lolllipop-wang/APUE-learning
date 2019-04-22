/**
 * ISO C规定一个进程最多可等级32个函数，这些函数将由exit()自动调用，
 * 函数的调用顺序与注册顺序相反，同一个函数被注册多次，也会多次调用。
 * 
 * #include <stdlib.h>
 * 
 * int atexit(void (*func)(void));
 * 返回值：若成功返回0，若失败返回非零
 * 
 * 使用atexit()函数登记终止处理函数（exit handler）
 */
#include <apue.h>

static void my_exit1();
static void my_exit2();

int main()
{
    if(atexit(my_exit2) != 0)
        err_sys("can't register my_exit2()");
    
    if(atexit(my_exit1) != 0)
        err_sys("cant't register my_exit1()");
    
    if(atexit(my_exit1) != 0)               /* 第二次注册my_exit1() */
        err_sys("cant't register my_exit1()");
    
    printf("main is done\n");
    exit(0);
}
static void my_exit1()
{
    printf("first exit handler\n");
}

static void my_exit2()
{
    printf("second exit handler\n");
}