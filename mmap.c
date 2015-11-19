#include<stdio.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<unistd.h>
#include <fcntl.h>

#define FILE_PATH "/home/billy/hmfsMount/a"

int main(void)
{
	int whence = SEEK_SET - 1;
	int fd, len = 200; // cannot be longer than file length
	off_t ofs;
	char *seq_write, inp;

	fd = open(FILE_PATH, O_RDWR);
	if (fd) {
		ofs = 0;
		seq_write = mmap(NULL, len, PROT_WRITE, MAP_SHARED, fd, 0);
		do{
			inp = getc(stdin);
			if(inp == '0'){
				break;
			}
			if(inp == '\n'){
				continue;
			}
			seq_write[ofs++]=inp;
		}while(ofs<len);
	}
	munmap(seq_write, len);
	close(fd);
	return 0;
}
