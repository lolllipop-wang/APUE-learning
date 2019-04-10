#include <apue.h>
#include <pthread.h>

void *run(void *arg)
{
    char *buf = (char *)arg;

    printf("%s", buf);

    return (void *)strlen(buf);
}

int
main()
{
    pthread_t th;
    void *res;
    int result;

    result = pthread_create(&th, NULL, run, "Hello World!\n");

    if(result != 0)
        err_sys("pthread_create error");
    
    printf("Massage from main\n");

    result = pthread_join(th, &res);

    if(result != 0)
        err_sys("pthread_join error");
    
    printf("Thread return value is %ld\n", (long)res);
    exit(0);
}