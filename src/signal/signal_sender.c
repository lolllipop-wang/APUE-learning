#include <apue.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    if(argc < 4)
        err_quit("usage: %s PID, num_sigs, sig_num [sig_num_2]");

    int num_sig, sig, i;
    pid_t pid;

    pid     = atol(argv[1]);
    num_sig = atoi(argv[2]);
    sig     = atoi(argv[3]);

    /**
     * 向PID为PID的进程发送sig信号sig_num次。
     */
    printf("%s: sending signal %d to process %ld %d times\n", argv[0], sig, pid, num_sig);

    for(i = 0; i < num_sig; ++i)
    {
        if(kill(pid, sig) == -1)
            err_sys("kill error");
    }

    /**
     * 如果存在第四个参数，将第四个信号表示的信号发送给指定进程。
     */
    if(argc > 4)
        if(kill(pid, atoi(argv[4])) == -1)
            err_sys("kill error");


    printf("process %s: exiting\n", argv[0]);

    exit(EXIT_SUCCESS);
}