#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  time_t t = time(NULL);

  printf("%s\n", ctime(&t));
  exit(0);
}