#include <stdio.h>
#include <setjmp.h>

static jmp_buf env;

static void
f2()
{
    longjmp(env, 2);
}

static void
f1(int argc)
{
    if(argc == 1)
        longjmp(env, 1);
    f2();
}

int
main(int argc, char *argv[])
{
    switch(setjmp(env)) {
        case 0: 
            printf("calling f1() after initialized setjmp\n");
            f1(argc);
            break;
        case 1:
            printf("jumped back from f1()\n");
            break;
        case 2:
            printf("jumped back from f2()\n");
            break;
        default:
            break;
    }
    return 0;
}