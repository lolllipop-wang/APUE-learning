#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
  time_t now;
  struct tm *local_time;

  char buf[63];
  char *index = "-\\|/\0";
  int i;
  buf[0] = '[';
  buf[61] = ']';
  buf[62] = '\0';

  for (i = 1; i < 61; i++)
    buf[i] = ' ';
  while (1) {

    printf("\033[?25l");
    time(&now);

    local_time = localtime(&now);

    int sec = local_time->tm_sec;
    if (sec == 0)
      sec = 60;
    for (i = 1; i < 61; i++) {
      if (i == sec && sec != 60)
        buf[i] = '>';
      else if (i == sec && sec == 60)
        buf[i] = '|';
      else if (i < sec)
        buf[i] = '=';
      else
        buf[i] = ' ';
    }

    printf("Today is %d/%02d/%02d %02d:%02d:%02d %s [%02ds][%c]\r",
           local_time->tm_year + 1900, local_time->tm_mon + 1,
           local_time->tm_mday, local_time->tm_hour, local_time->tm_min, sec,
           buf, sec, index[sec % 4]);
    fflush(stdout);
    sleep(1);
  }
  printf("\n");
  return 0;
}
