/**
 * 定时器与休眠
 * 为read设置超时，如果read阻塞时间超过设定时间，则会中断read
 */
#include <apue.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#define MAXBUFFSIZE 4096

char buf[MAXBUFFSIZE];

static void handler(int sig) { printf("\ncaught a signal\n"); }

int main(void) {
  struct sigaction sa;
  int num_read;
  int time_out = 3;
  int saved_errno;

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;

  if (sigaction(SIGALRM, &sa, NULL) == -1)
    err_sys("setting SIGALRM handler error");

  printf("please enter a string, or it will time out int 3 seconds: ");
  fflush(stdout);

  alarm(time_out);
  num_read = read(STDIN_FILENO, buf, MAXBUFFSIZE - 1);

  saved_errno = errno;
  alarm(0);
  errno = saved_errno;

  if (num_read == -1) {
    if (errno == EINTR)
      printf("read time out\n");
    else
      err_sys("read error");
  } else {
    printf("successful read %d bytes\n", num_read);
    buf[num_read] = '\0';
    printf("\tresult is: %s\n", buf);
  }

  exit(0);
}