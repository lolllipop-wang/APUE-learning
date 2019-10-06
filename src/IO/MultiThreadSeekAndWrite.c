/**
 *
 * 按照书上的要求，同时启动两个进程
 * X模式(不加O_APPEND标志)写入文件后，文件内容少于20行，
 * 说明在lseek和write之间又其他线程写入，覆盖了其他线程写入的内容。
 */
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct my_thread {
  int id;
  pthread_t th;
  int argc;
  char **argv;
};

typedef struct my_thread my_thread;

void *run(void *args);

int main(int argc, char *argv[]) {

  my_thread thread[10];
  int i;
  for (i = 0; i < 10; i++) {
    thread[i].argc = argc;
    thread[i].argv = argv;
    thread[i].id = i;
    pthread_create(&thread[i].th, NULL, run, &thread[i]);
  }

  for (i = 0; i < 10; ++i) {
    pthread_join(thread[i].th, NULL);
  }

  exit(EXIT_SUCCESS);
}

void *run(void *args) {
  my_thread *thread = (my_thread *)args;

  int argc = thread->argc;
  char **argv = thread->argv;

  if (argc < 3) {
    fprintf(stderr, "usage: %s filename buffer [x]\n", argv[0]);
    exit(1);
  }

  int fd = 0;
  int no_x_open_oflags = O_RDWR | O_CREAT | O_APPEND;
  int x_open_oflags = O_RDWR | O_CREAT;
  mode_t mode = S_IRUSR | S_IWUSR;
  ssize_t num_write;
  char buf[100];
  sprintf(buf, "process: %ld, thread : %d, buf: %s\n", (long)getpid(),
          thread->id, argv[2]);
  size_t buf_length = strlen(buf);

  switch (argc) {
  case 3:
    printf("write data in normal mode!\n");
    if ((fd = open(argv[1], no_x_open_oflags, mode)) < 0) {
      fprintf(stderr, "open file error\n");
      exit(1);
    }

    break;
  case 4:
    printf("write data in X mode!\n");
    if (argv[3][0] == 'X' || (fd = open(argv[1], x_open_oflags, mode)) < 0) {
      fprintf(stderr, "open file error\n");
      exit(1);
    }

    if (lseek(fd, 0, SEEK_END) < 0) {
      fprintf(stderr, "seek error\n");
      exit(1);
    }

    break;
  default:
    fprintf(stderr, "usage: %s filename buffer [x]\n", argv[0]);
    break;
  }

  num_write = write(fd, buf, buf_length);
  if (num_write < 0) {
    fprintf(stderr, "write error\n");
    exit(1);
  }

  if (num_write < buf_length)
    printf("write fewer bytes\n");
  printf("write %lld bytes\n", (long long)num_write);
  pthread_exit(NULL);
}
