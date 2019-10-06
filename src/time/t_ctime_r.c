#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  time_t t = time(NULL);
  char *buf = (char *)malloc(sizeof(char) * 30);
  ctime_r(&t, buf);
  printf("%s\n", buf);
  return 0;
}