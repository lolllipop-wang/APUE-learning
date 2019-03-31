#include <apue.h>
#include <fcntl.h>

char buf[16440];

int main()
{
	int fd;
	if((fd = creat("file.nohole", FILE_MODE)) < 0)
		err_sys("create file error");

	if(write(fd, buf, 16394) != 16394)
		err_sys("write error");
	return 0;
}
