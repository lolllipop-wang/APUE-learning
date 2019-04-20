#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
    time_t t = time(NULL);

    printf("%s\n", ctime(&t));
    exit(0);
}