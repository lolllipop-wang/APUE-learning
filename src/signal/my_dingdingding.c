/**
 * 定时器与休眠
 * 每隔sec秒叮叮叮。。。叮叮叮一次，共num次结束。
 */
#include <apue.h>
#include <sys/time.h>
#include <time.h>

static volatile sig_atomic_t got_alarm = 0;

static void handler(int sig) { got_alarm = 1; }

int main(int argc, char *argv[]) {
  if (argc != 3)
    err_quit("usage: %s sec num");

  int max_sigs;
  int sig_count;
  clock_t perv_clock;
  struct sigaction sa;
  struct itimerval itv;

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;

  if (sigaction(SIGALRM, &sa, NULL) < 0)
    err_sys("setting SIGALRM handler error");

  itv.it_value.tv_sec = atoi(argv[1]);
  itv.it_value.tv_usec = 0;
  itv.it_interval.tv_sec = 1;
  itv.it_interval.tv_usec = 0;

  if (setitimer(ITIMER_REAL, &itv, NULL) == -1)
    err_sys("setitmer error");

  max_sigs = atoi(argv[2]);
  sig_count = 0;
  perv_clock = clock();
  printf("Start: sec = %d, nums = %d\n", itv.it_value.tv_sec, max_sigs);

  for (;;) {
    while (((clock() - perv_clock) * 10 / CLOCKS_PER_SEC) < 5) {
      if (got_alarm) {
        got_alarm = 0;
        printf("Alarm: 叮叮叮。。。叮叮叮\n");
        sig_count++;
        if (sig_count >= max_sigs) {
          printf("时间到了！\n");
          exit(0);
        }
      }
    }
    perv_clock = clock();
    printf("Waitting。。。\n");
  }

  exit(0);
}