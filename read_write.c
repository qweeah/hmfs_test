#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

#define FILE_PATH "/home/billy/hmfsMount/a"
#define BUFF_SIZE ((1 << 10) -1)
#define MAX_SIZE 20

char *r_buff, *w_buff;
FILE *file;
int fd;

int seq_rand_read_write()
{
	off_t pos1, pos2, pos3;
	int pos, i;
	int r_len, w_len, r_w_len;

	pos = ((int)rand()) % MAX_SIZE + 1;
	r_len = ((int)rand()) % MAX_SIZE + 1;

	pos1 = lseek(fd, pos, SEEK_SET);
	w_len = read(fd, r_buff, r_len);
	pos2 = lseek(fd, 0, SEEK_END);
	write(fd, r_buff, w_len);
	pos3 = lseek(fd, pos2, SEEK_SET);
	r_w_len = read(fd, w_buff, w_len);
	for (i = 0; i < r_w_len; i++) {
		if (r_buff[i] != w_buff[i] || w_buff == NULL) {
			printf(" === error at %d ====\n", pos);
			printf(" w_buff:%s\n", w_buff);
			printf(" ====================\n");
			printf(" r_buff:%s\n", r_buff);
			printf(" ========stop========\n\n");
			return 1;
		}
	}
	return 0;
}

int seq_rand_read(void)
{
	int pos, i;
	int pos1, pos2;
	int r_len, w_len, r_w_len;

	pos = ((int)rand()) % MAX_SIZE + 1;
	r_len = ((int)rand()) % MAX_SIZE + 1;

	pos1 = lseek(fd, pos, SEEK_SET);
	w_len = read(fd, r_buff, r_len);
	printf(" r_buff:%s\n", r_buff);
	return 0;
}

int main(void)
{
	long long count;
	int i;

	file = fopen(FILE_PATH, "a+");
	fd = fileno(file);
	if (fd) {
		r_buff = malloc(BUFF_SIZE);
		w_buff = malloc(BUFF_SIZE);
		printf("enter number of r/w test:");
		scanf("%lld", &count);
		while (count--) {
			if (seq_rand_read_write()) {
				printf("%lld\n", count);
				break;
			}
			if (count == 9000) {
				printf("9k left!\n");
			}
		}
	}
	printf("r/w %d times, no error found!\n");
	fclose(file);

	free(r_buff);
	free(w_buff);
	return 0;
}
