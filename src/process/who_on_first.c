#include <apue.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{

    int num_child = 10;
    int i;
    setbuf(stdout, NULL); /* stdout unbuffered */

    for(i = 0; i < num_child; ++i) {
        switch (fork())
        {
            case -1:
                /* code */
                err_sys("fork error in %d\n", i);
                break;
            case 0: 
                printf("child %d\n", i);
                _exit(0);
            default:
                printf("parent %d\n", i);
                //wait(NULL);
                break;
        }
    }
    exit(0);
}