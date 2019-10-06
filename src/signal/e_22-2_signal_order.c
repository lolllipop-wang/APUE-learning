/**
 * 测试Linux实时信号和标准信号同时等待进程时信号的传递顺序。
 */
#include <apue.h>

static volatile int order_count = 0; /* 标记信号到达的次序 */

/**
 * 标准信号SIGINT的信号处理函数
 */
static void standard_signal_handler(int sig) {
  printf("%d: caught standard signal: %d(%s)\n", ++order_count, sig,
         strsignal(sig));
}

/**
 * 实时信号SIGUSR1的信号处理函数
 */
static void real_time_signal_handler(int sig) {
  printf("%d: caught real time signal: %d(%s)\n", ++order_count, sig,
         strsignal(sig));
}

int main(int argc, char *argv[]) {
  /**
   * 打印进程的PID，方便在另一个终端窗口内使用kill向本进程发送信号
   */
  printf("%s: PID = %ld, UID = %ld\n", argv[0], (long)getpid(), (long)getuid());

  sigset_t mask;
  struct sigaction sa_standard_signal;
  struct sigaction sa_real_time_signal;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGUSR1);

  printf("blocking SIGINT and SIGUSR1...\n");
  if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0) /* 阻塞SIGINT 和SIGUSR1*/
    err_sys("blocking signal error");

  sa_standard_signal.sa_flags = 0;
  sa_standard_signal.sa_handler = standard_signal_handler;
  sigfillset(&sa_standard_signal.sa_mask);

  sa_real_time_signal.sa_flags = 0;
  sa_real_time_signal.sa_handler = real_time_signal_handler;
  sigfillset(&sa_real_time_signal.sa_mask);

  printf("setting up standard signal SIGINT handler...\n");
  if (sigaction(SIGINT, &sa_standard_signal, NULL) < 0)
    err_sys("setting SIGINT handler error");

  printf("setting up real time signal SIGUSR1 handler...\n");
  if (sigaction(SIGUSR1, &sa_real_time_signal, NULL) < 0)
    err_sys("setting SIGUSR1 handler error");

  sleep(20);

  sigemptyset(&mask);
  printf("unblocking SIHINT and SIGUSR1...\n");
  if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0)
    err_sys("unblocking signal error");
  for (;;)
    pause();
  exit(0);
}