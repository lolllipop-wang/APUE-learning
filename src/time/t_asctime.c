#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  time_t t = time(NULL);
  struct tm *local_time = localtime(&t);
  printf("%s\n", asctime(local_time));
  return 0;
}