#include <apue.h>
#include <errno.h>
#include <pthread.h>

void *run(void *arg) {
  if (pthread_join(pthread_self(), NULL) != 0)
    err_sys("pthread_join error");
  return NULL;
}

int main() {
  pthread_t th;
  void *res;
  int result;

  result = pthread_create(&th, NULL, run, "Hello World!\n");

  if (result != 0)
    err_sys("pthread_create error");
  pthread_exit(NULL);
  exit(0);
}