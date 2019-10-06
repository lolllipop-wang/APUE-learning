#include <apue.h>
#include <crypt.h>

static char *str;
static volatile int handled = 0;

static void my_signal_handler(int sig) {
  crypt(str, "XX");
  handled++;
}

int main(int argc, char *argv[]) {
  struct sigaction sa;

  if (argc != 3)
    err_sys("usage: %s str1, str2", argv[0]);

  str = argv[1];

  char *strcrypt = strdup(crypt(argv[2], "XX"));

  sa.sa_handler = my_signal_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  int miss_match = 0;
  int call_num = 0;
  if (sigaction(SIGINT, &sa, NULL) < 0)
    err_sys("sigaction error");

  for (;; ++call_num) {
    if (strcmp(strcrypt, crypt(argv[2], "XX")) != 0) {
      miss_match++;
      printf("miss match on call %d (miss_match = %d, handled = %d)\n",
             call_num, miss_match, handled);
    }
  }

  exit(EXIT_SUCCESS);
}