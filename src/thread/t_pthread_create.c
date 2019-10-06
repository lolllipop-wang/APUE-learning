#include <apue.h>
#include <pthread.h>

struct my_thread {
  pthread_t th;
  int num;
};

typedef struct my_thread my_thread;

void *run(void *arg) {
  my_thread *ptr = (my_thread *)arg;
  printf("Thread %d is running!\n", ptr->num);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  my_thread thread[10];
  int i, result;
  for (i = 0; i < 10; ++i) {
    thread[i].num = i;
    result = pthread_create(&thread[i].th, NULL, run, &thread[i]);
    if (result != 0)
      err_sys("pthread_create error");
  }
  pthread_exit(NULL);
  exit(0);
}