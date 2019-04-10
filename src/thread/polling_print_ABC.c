#include <apue.h>
#include <pthread.h>

static volatile int i = 1, j = 10;

void *
run1(void *arg)
{
    while(j > 0) {
        while(i != 1);
        printf("A");
        i = 2;
        j--;
    }
    pthread_exit(NULL);
}

void *
run2(void *arg)
{
    while(j > 0) {
        while(i != 2);
        printf("B");
        i = 3;
        j--;
    }
    pthread_exit(NULL);
}

void *
run3(void *arg)
{
    while(j > 0) {
        while(i != 3);
        printf("C");
        i = 1;
        j--;
    }
    puts("");
    pthread_exit(NULL);
}

int
main()
{
    pthread_t thread[3];
    void *(* run[3])(void *) = {run1, run2, run3};
    int i = 0, result;
    for(i = 0; i < 3; ++i) {
        result = pthread_create(&thread[i], NULL, run[i], NULL);
        if(result != 0)
            err_sys("pthread_create error");
    }

    pthread_exit(NULL);
    exit(0);
}