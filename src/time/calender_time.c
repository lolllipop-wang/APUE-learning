#include <time.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <sys/time.h>

#define SECONDS_IN_TOPICAL_YESR (365.24219 * 24 * 60 * 60)
int main()
{
    time_t t;
    struct tm *gmp, *locp;
    struct tm gm, loc;
    struct timeval tv;

    t = time(NULL);
    printf("Seconds  since th Epoch (1 Jan 1970): %ld", (long)t);
    printf(" (about %6.3f years)\n", t / SECONDS_IN_TOPICAL_YESR);

    if(gettimeofday(&tv, NULL) == -1) {
        fprintf(stderr, "gettimeofday error\n");
        exit(1);
    }

    printf("gettimeofday() returned %ld seconds, %ld microseconds\n", 
                (long)tv.tv_sec, (long)tv.tv_usec);
    
    gmp = gmtime(&t);

    if(gmp == NULL) {
        fprintf(stderr, "gmtime error");
        exit(1);
    }

    gm = *gmp;
    printf("Broken down by gmtime():\n");
    printf("  year = %d mon = %d mday = %d hour = %d min = %d sec = %d ", 
                gm.tm_year, gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
    printf("wday = %d yday = %d isdst = %d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

    locp = localtime(&t);
    if(locp == NULL) {
        fprintf(stderr, "localtime error");
        exit(1);
    }

    loc = *locp;

    printf("Broken down by localtime():\n");
    printf("  year = %d mon = %d mday = %d hour = %d min = %d sec = %d ", 
                gm.tm_year, gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
    printf("wday = %d yday = %d isdst = %d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

    printf("asctme() formats the gmtime() values as: %s\n", asctime(&gm));
    printf("ctime() formats time() values as: %s\n", ctime(&t));

    printf("mktime() of gmtime() value:     %ld secs\n", (long)mktime(&gm));
    printf("mktime() of localtime() values: %ld secs\n", (long)mktime(&loc));
    return 0;
}