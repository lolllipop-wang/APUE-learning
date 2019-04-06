#include <apue.h>
#include <fcntl.h>

#define BUFFSIZE 4096

char buf[BUFFSIZE];

int main(int argc, char *argv[])
{
	if(argc < 3)
		err_sys("usage: ReadAndWrite in out");

	int InputFileNO, OutputFileNO;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	if((InputFileNO = open(argv[1], O_RDONLY)) < 0)
		err_sys("open input file error");
		
	if((OutputFileNO = open(argv[2], O_RDWR | O_CREAT | O_SYNC, mode)) < 0)
		err_sys("open output file error");
	
	int n;
	while((n = read(InputFileNO, buf, BUFFSIZE)) > 0)
		if(write(OutputFileNO, buf, n) != n)
			err_sys("write error");
	if(n < 0)
		err_sys("read error");
	return 0;	
}
