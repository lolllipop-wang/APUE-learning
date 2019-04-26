/**
 * 显示进程记账文件中的数据。
 * 
 */
#include <apue.h>
#include <fcntl.h>
#include <sys/acct.h>

#define LINUX
#define HAS_AXSIG
#define HAS_ACORE

#if defined(BSD)
#define acct acctv2
#define ac_flag ac_trailer.ac_flag
#define FMT "%-*.*s e = %.0f, chars %.0f, %c %c %c %c\n"
#elif defined(HAS_AC_STAT)
#define FMT "%-*.*s e = %6ld, chars = %7ld, stat = %3u: %c %c %c %c\n"
#else
#define FMT "%-*.*s e = %6ld, chars = %7ld, %c %c %c %c\n"
#endif

#if defined(LINUX)
#define acct acct_v3
#endif

#if !defined(HAS_ACORE)
#define ACORE 0
#endif

#if !defined(HAS_AXSIG)
#define AXSIG 0
#endif

#if !defined(BSD)
static unsigned long compt_to_ulong(comp_t comptime)
{
    unsigned long   val;
    int             exp;

    val = comptime & 0x1fff;
    exp = (comptime >> 13) & 7;
    while(exp-- > 0)
        val *= 8;
    
    return val;
}
#endif

int main(int argc, char *argv[])
{
    int         fd;
    int         nread;
    struct acct acc;

    if(argc != 2)
        err_quit("usage: %s accfile", argv[0]);
    
    fd = open(argv[1], O_RDWR);

    if(fd < 0)
        err_sys("%s: can't open file %s", argv[0], argv[1]);
    
    while((nread = read(fd, &acc, sizeof(struct acct)))) {
        if(nread < sizeof(struct acct))
            err_quit("%s: partial read", argv[0]);
        
        printf(FMT, (int)sizeof(acc.ac_comm),
                (int)sizeof(acc.ac_comm), acc.ac_comm,
#if defined(BSD)
                acc.ac_etime, acc.ac_io,
#else
               compt_to_ulong(acc.ac_etime), compt_to_ulong(acc.ac_io),
#endif
#if defined(HAS_AC_STAT)
                (unsigned char) acc.ac_stat,
#endif
               acc.ac_flag & ACORE ? 'D' : '-',
               acc.ac_flag & AXSIG ? 'X' : '-',
               acc.ac_flag & AFORK ? 'F' : '-',
               acc.ac_flag & ASU   ? 'S' : '-');
    }

    if(nread < 0)
        err_sys("%s: can't read %s", argv[0], argv[1]);
    
    exit(0);
}