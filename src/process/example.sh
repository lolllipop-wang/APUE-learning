#! /usr/bin/sh

case `uname -s` in
"FreeBSD")
    PLANTFORM="freebsd"
    ;;
"Linux")
    PLANTFORM="linux"
    ;;
"Darwin")
    PLANTFORM="macos"
    ;;
"SunOS")
    PLANTFORM="solaris"
    ;;
*)
    echo "Unknown plantform" >&2
    exit 1
esac
echo $PLANTFORM
exit 0