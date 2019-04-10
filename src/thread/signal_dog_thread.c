#include <apue.h>
#include <pthread.h>

void *
run(void *arg)
{
    int res;
    printf("Before I become a signal dog.\n");
    res = pthread_detach(pthread_self());
    if(res != 0)
        err_sys("Oh! shit...");
    printf("I'm a signal dog now.\n");
    pthread_exit(NULL);
}

int
main(int argc, char *argv[])
{
    pthread_t th;
    int res;
    setbuf(stdout, NULL);
    res = pthread_create(&th, NULL, run, NULL);
    if(res != 0)
        err_sys("pthread_create error");
    pthread_exit(NULL);
    exit(0);
}