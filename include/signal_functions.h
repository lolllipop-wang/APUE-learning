#ifndef SIGNAL_FUNCTIONS_H
#define SIGNAL_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#define _GUN_SOURCE

#include <stdio.h>
#include <string.h>
#include <signal.h>
void
print_sigset(FILE *of, const char *prefix, const sigset_t *segset)
{
    int sig, cnt = 0;
    for(sig = 1; sig < NSIG; ++sig)
    {
        if(sigismember(segset, sig) == 1)
        {
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
            ++cnt;
        }
    }

    if(cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}

int
print_sig_mask(FILE *of, const char *msg)
{
    sigset_t curr_mask;

    if(msg != NULL)
        fprintf(of, "%s", msg);

    if(sigprocmask(SIG_BLOCK, NULL, &curr_mask) == -1)
        return -1;

    print_sigset(of, "\t\t", &curr_mask);
    return 0;
}

int
print_pending_sigs(FILE *of, const char *msg)
{
    sigset_t pending_sigs;

    if(msg != NULL)
        fprintf(of, "%s", msg);
    
    if(sigpending(&pending_sigs) == -1)
        return -1;
    
    print_sigset(of, "\t\t", &pending_sigs);
    return 0;

}

#ifdef __cplusplus
}
#endif

#endif