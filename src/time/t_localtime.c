#define _BSD_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct tm *local_time;
    time_t t = time(NULL);
    local_time = localtime(&t);
    printf("        Time Zone: %s\n", local_time -> tm_zone);
    printf("          Seconds: %d\n", local_time -> tm_sec);
    printf("          Minutes: %d\n", local_time -> tm_min + 1);
    printf("             Hour: %d\n", local_time ->tm_hour);
    printf(" Day of the Month: %d\n", local_time -> tm_mday);
    printf("  Day of the Week: %d\n", local_time -> tm_wday);
    printf("           Mounth: %d\n", local_time -> tm_mon + 1);

    exit(0);
}