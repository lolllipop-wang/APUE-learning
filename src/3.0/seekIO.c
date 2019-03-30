#include <apue.h>
#include <fcntl.h>
																																																													
int main(int argc, char *argv[])
{
	if(argc < 3) {
		err_sys("Usage: %s file {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);
	}

	int fd, n, ap, len, i;
	char *buf;
	mode_t mod = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	if((fd = open(argv[1], O_RDWR | O_CREAT, mod)) == -1) {
		err_sys("open file error");
	}
	for(ap = 2; ap < argc; ++ap) {
		switch(argv[ap][0]) {
			case 'r':
			case 'R':
				len = atol(argv[ap] + 1);
				buf = (char *)malloc(len);
				if(buf == NULL) {
					err_sys("malloc error");
				}
				if((n = read(fd, buf, len)) < 0) {
					err_sys("read error");
				}
				if(n == 0) {
					err_sys("end of file");
				}

				printf("%s: ", argv[ap]);

				for(i = 0; i < n; ++i) {
					if(argv[ap][0] == 'r')
						printf("%c", buf[i]);
					else
						printf("%02x ", buf[i]);
				}
				printf("\n");
				free(buf);
				break;
			case 'w':
				if((n = write(fd, argv[ap] + 1, strlen(argv[ap] + 1))) < 0)
					err_sys("write error");
				printf("%s : worte %ld butes\n", argv[ap], (long)n);
				break;
			default:
				err_sys("We have a big truble");
		}
	}
	exit(0);
}
