#include <stdio.h>
#include <setjmp.h>

static jmp_buf env;

static void
do_jump(int nvar, int rvar, int vvar)
{
    printf("inside do_jump, nvar = %d, rvar = %d, vvar = %d\n", nvar, rvar, vvar);
    longjmp(env, 1);
}

int
main(int argc, char *argv[])
{
             int nvar = 111;
    register int rvar = 222;
    volatile int vvar = 333;

    if(setjmp(env) == 0) {
        nvar = 777;
        rvar = 888;
        vvar = 999;

        do_jump(nvar, rvar, vvar);
    } else {
        printf("do_jump back, nvar = %d, rvar = %d, vvar = %d\n", nvar, rvar, vvar);
    }
    return 0;
}