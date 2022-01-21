/* mycp program, copy a text to new file, by Hyungilee, lee991229@naver.com/2021-10-11 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#define MAX_BUF 64

int main(int argc, char *argv[])
{
    int fd1, fd2, read_size, write_size;
    char buf[MAX_BUF];
    struct stat st1;

    if(argc!=3) {
	printf("USAGE:%s source_file destination_file \n", argv[0]);
	exit(-1);
    }
    fd1=open(argv[1], O_RDONLY);
    if(fd1<0) {
	printf("Can't open %s. No such file\n", argv[1]); 
	exit(-1);
    }
#define STAT_
#ifndef STAT_
    fd2=open(argv[2], O_WRONLY | O_CREAT | O_EXCL);
#else
    fstat(fd1,&st1);
    fd2=open(argv[2], O_WRONLY | O_CREAT | O_EXCL, st1.st_mode);
#endif
    if(fd2<0) {
	printf("Can't open %s. May already exist!\n", argv[2]);
    }
    while(1) {
    read_size=read(fd1,buf,MAX_BUF);
    if(read_size==0)
	break;
    write_size=write(fd2,buf,read_size);
    }
    close(fd2);
    close(fd1);

    return 0;  
}
