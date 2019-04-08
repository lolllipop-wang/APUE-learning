#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "signal_functions.h"

int
main(int argc, char *argv[])
{
    sigset_t sigset;
    sigemptyset(&sigset);
    sigfillset(&sigset);
    print_sigset(stdout, "Signal: ", &sigset);
    exit(EXIT_SUCCESS);
}