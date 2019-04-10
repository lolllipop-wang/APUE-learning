#include <apue.h>
#include <pthread.h>

static int global = 0;

void *
run(void *arg)
{
    int loop = *(int *)arg;
    int i, local;
    for(i = 0; i < loop; ++i) {
        local = global;
        local++;
        global = local;
    }

    return NULL;
}

int
main(int argc, char *argv[])
{
    if(argc < 2)
        err_sys("usage: %s loop_nums", argv[0]);
    int loop, i, result;
    pthread_t thread[10];

    loop = atoi(argv[1]);
    for(i = 0; i < 10; ++i) {
        result = pthread_create(&thread[i], NULL, run, (void *)&loop);
        if(result != 0)
            err_sys("pthread_create error");
    }

    for(i = 0; i < 10; ++i) {
        result = pthread_join(thread[i], NULL);
        if(result != 0)
            err_sys("pthread_join error");
    }
    
    printf("global = %d\n", global);

    exit(0);
}