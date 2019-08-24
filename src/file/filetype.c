/**
 * 打印文件的类型
 * 1. 普通文件(Regular File)
 * 2. 目录文件(Directory File)
 * 3. 块特殊文件(Block Special File)
 * 4. 字符特殊文件(Character Special File)
 * 5. FIFO(Named Pipe)
 * 6. 套接字(Socket)
 * 7. 符号连接(Symbolic Link)
 * 
 * #define S_ISDIR(mode) (((node) & S_IFMT) == S_IFDIR)
 */
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int i;
    struct stat file;

    for(i = 1; i < argc; ++i) {
        printf("%s: ", argv[i]);

        if(lstat(argv[i], &file) < 0) {
            err(EXIT_FAILURE, "%s: lstat error", argv[0]);
            continue;
        }

        if(S_ISREG(file.st_mode))
            puts("Regular File");

        else if(S_ISDIR(file.st_mode))
            puts("Directory File");

        else if(S_ISBLK(file.st_mode))
            puts("Block Special File");

        else if(S_ISCHR(file.st_mode))
            puts("Character Special File");

        else if(S_ISFIFO(file.st_mode))
            puts("FIFO");

        else if(S_ISSOCK(file.st_mode))
            puts("Socket");

        else if(S_ISLNK(file.st_mode))
            puts("Symbolic Link");

        else 
            puts("Unknown File Type");
    }
    exit(0);
}