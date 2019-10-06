/**
 * 实现abort。
 */
#include <apue.h>

static volatile sig_atomic_t can_abort = 0;

static void my_abort() {
  struct sigaction sa;

  /**
   * 阻塞SIGABRT除外的所有信号，abort不需要返回。
   */
  sigfillset(&sa.sa_mask);
  sigdelset(&sa.sa_mask, SIGABRT);
  sigprocmask(SIG_SETMASK, &sa.sa_mask, NULL);
  raise(SIGABRT);

  /**
   * 如果上面的操作没有成功，将信号处理函数置为SIG_DEL，
   * 再次尝试。
   */
  sa.sa_handler = SIG_DFL;
  sa.sa_flags = 0;
  sigfillset(&sa.sa_mask);
  sigaction(SIGABRT, &sa, NULL);
  sigdelset(&sa.sa_mask, SIGABRT);
  sigprocmask(SIG_SETMASK, &sa.sa_mask, NULL);
  raise(SIGABRT);
  exit(1);
}

static void handler(int sig) {
  printf("Caught a signal %d(%s)\n", sig, strsignal(sig));
  can_abort = 1;
}

int main() {
  struct sigaction sa;
  sa.sa_handler = handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGINT, &sa, NULL) == -1)
    err_sys("sigaction error");

  printf("Ctrl C to abort\n");
  for (;;) {
    if (can_abort == 1)
      my_abort();
  }

  exit(0);
}