#define _GNU_COURCE
#include <apue.h>
#include <signal.h>
#include "../../include/signal_functions.h"

static int sig_cnt[NISG];

static sig_atomic_t sig_int = 0;

static void
handler(int sig)
{
    if(sig == SIGINT) {
        sig_int = 1;
    }
    else {
        sig_cnt[sig]++;
    }
}

int
main(int argc, char *argv[])
{
    
    exit(EXIT_SUCCESS);
}