#include <cstdio>
#include <cstdlib>
#include <err.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/event.h>

#define MAXSIZE 100

bool Register(int kq, const char *filename) {
    int fd, ret;
    struct kevent event;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
        return false;

    EV_SET(&event, fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, (void *)filename);

    ret = kevent(kq, &event, 1, NULL, 0, NULL);
    if (ret == -1 || (event.flags & EV_ERROR))
        return false;

    return true;
}

int main(int argc, char *argv[]) {
    int kq, ret, numRegistered;
    struct kevent eventList[MAXSIZE];

    if (argc < 2)
        err(EXIT_FAILURE, "Usage: %s filename1, filename2, filename3, ...", argv[0]);

    numRegistered = 0;

    kq = kqueue();
    if (kq == -1)
        err(EXIT_FAILURE, "Failed create kqueue!");

    for (int i = 1; i < argc; ++i) {

        if (!Register(kq, argv[1])) {
            err(EXIT_FAILURE, "Failed register file %s", argv[i]);
        } else {
            printf("Register file %s success!\n", argv[i]);
            numRegistered++;
        }
    }

    for (;;) {

        ret = kevent(kq, nullptr, 0, eventList, numRegistered, nullptr);

        if (ret == -1) {
            err(EXIT_FAILURE, "Error on kqueue wait");
        } else {

            for (int i = 0; i < ret; ++i)
                printf("Something was written in '%s'\n", eventList[i].udata);
        }
    }

    return 0;
}