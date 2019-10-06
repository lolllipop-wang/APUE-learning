#include <apue.h>
#include <pthread.h>

static int global = 0;
static pthread_mutex_t mutex;

void *run(void *arg) {
  int loop = *(int *)arg;
  int i, local, result;
  result = pthread_mutex_lock(&mutex);
  if (result != 0)
    err_sys("pthread_mutex_lock error");
  for (i = 0; i < loop; ++i) {
    local = global;
    local++;
    global = local;
  }
  result = pthread_mutex_unlock(&mutex);
  if (result != 0)
    err_sys("pthread_mutex_unlock error");
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    err_sys("usage: %s loop_nums", argv[0]);
  int loop, i, result;
  pthread_t thread[10];
  pthread_mutexattr_t mutex_attr;

  result = pthread_mutexattr_init(&mutex_attr);
  if (result != 0)
    err_sys("pthread_mutexattr_init error");

  result = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
  if (result != 0)
    err_sys("pthread_mutexattr_settype error");

  result = pthread_mutex_init(&mutex, &mutex_attr);
  if (result != 0)
    err_sys("pthread_mutex_init error");

  result = pthread_mutexattr_destroy(&mutex_attr); /* No longer needed */
  if (result != 0)
    err_sys("pthread_mutexattr_destory error");

  loop = atoi(argv[1]);
  for (i = 0; i < 10; ++i) {
    result = pthread_create(&thread[i], NULL, run, (void *)&loop);
    if (result != 0)
      err_sys("pthread_create error");
  }

  for (i = 0; i < 10; ++i) {
    result = pthread_join(thread[i], NULL);
    if (result != 0)
      err_sys("pthread_join error");
  }
  printf("Expectation global is: %d\n", 10 * loop);
  printf("global = %d\n", global);

  result = pthread_mutex_destroy(&mutex);

  if (result != 0)
    err_sys("pthread_mutex_destory error");

  exit(0);
}