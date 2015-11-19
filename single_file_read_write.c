#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

#define FILE_PATH "/home/billy/hmfsMount/a"
#define BUFF_SIZE ((1 << 10) -1)
#define MAX_SIZE 20
#define THREAD_COUNT 20

char *r_buff, *w_buff;
FILE *file;
int fd;
long long count;
int *done;

pthread_t *t;

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

void *con_rand_read_write(void *arg)
{
	off_t pos1, pos2, pos3;
	int pos, i;
	int r_len, w_len, r_w_len;

	pos = ((int)rand()) % MAX_SIZE + 1;
	r_len = ((int)rand()) % MAX_SIZE + 1;

	pos1 = lseek(fd, pos, SEEK_SET);
	w_len = read(fd, r_buff, r_len);
	pos2 = lseek(fd, 0, SEEK_END); //XXX: lock free should cause error
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
			return NULL;
		}
	}
	(*done)++;
	if(*done == count){
		printf("no error found in concurrent RW!\n");
	//	exit(3);
	} else {
		//printf("done:%d\n", *done);
	}
	//exit(3);
	return NULL;
}

int main(void)
{
	int i;
	int err;

	file = fopen(FILE_PATH, "a+");
	fd = fileno(file);
	if (fd) {
		r_buff = malloc(BUFF_SIZE);
		w_buff = malloc(BUFF_SIZE);
		printf("enter number of seq r/w test:");
		scanf("%lld", &count);
		while (count--) {
			if (seq_rand_read_write()) {
				printf("error in %lld times r/w\n", count);
				goto free_seq;
			}
			if (count == 9000) {
				printf("9k left!\n");
			}
		}
	}
	printf("no error found in seq RW!\n");

	goto free_seq;

	printf("enter number of seq r/w test:");
	scanf("%lld", &count);
	done = malloc(sizeof(int));
	t = malloc(sizeof(pthread_t) * count);

	*done = 0;
	for(i=0; i<count; i++){
		err = pthread_create(t+i, NULL, &con_rand_read_write, (void*)done);
		if (err != 0) {
			printf("can't create thread :[%d]\n", i);
			goto free_con;
		}
	}
	for(i=0; i<count; i++){
		pthread_join(t[i], NULL);
	}
free_con:
	free(done);
free_seq:
	free(r_buff);
	free(w_buff);
	fclose(file);
	printf("free done!\n");
	return 0;
}
