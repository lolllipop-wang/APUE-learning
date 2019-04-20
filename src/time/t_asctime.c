#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    time_t t = time(NULL);
    struct tm *local_time = localtime(&t);
    printf("%s\n", asctime(local_time));
    return 0;
}