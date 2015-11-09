#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

#define FILE_PATH "/home/billy/hmfsMount/a"

/*
 *     0        SEEK_SET
 *     1        SEEK_CUR
 *     2        SEEK_END
 */

int main(void)
{
	int whence = SEEK_SET - 1;
	FILE *file;
	int fd;
	off_t res, ofs;

	file = fopen(FILE_PATH, "w");
	printf("user file:%p\n", file);
	fd = fileno(file);
	if (fd) {
		ofs = 10;
		while (whence++ < SEEK_END) {
			res = lseek(fd, ofs, whence);
			printf("res:%d\n", (int)res);
			ofs++;
		}
	}
	return 0;
}
