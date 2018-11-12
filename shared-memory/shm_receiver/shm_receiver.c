#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>

#define MSG_MAX_LEN 4096 // 4KB max message length mapped in the memory

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[]) {
	int shm_fd;
	sem_t *sem_r;
	sem_t *sem_w;
	
	char *msg;

	char *options="m:w:r:";

	char *shm_name;
	char *sem_name_write;
	char *sem_name_read;

	char c;
	while((c=getopt(argc, argv, options)) != -1) {
		switch(c) {
			case 'm':
				shm_name=optarg;
				break;
			case 'w':
				sem_name_write=optarg;
				break;
			case 'r':
				sem_name_read=optarg;
				break;
		}
	}

	sem_unlink(sem_name_write);
	sem_unlink(sem_name_read);
	shm_unlink(shm_name);

	shm_fd=shm_open(shm_name, O_RDWR|O_CREAT|O_TRUNC, 0660);
	ftruncate(shm_fd, MSG_MAX_LEN);

	if(shm_fd < 0) {
		error("shm_open(): cant open.");
	}

	sem_r=sem_open(sem_name_read, O_RDONLY|O_CREAT, 0660, 0);

	if(sem_r < 0) {
		error("sem_open(): cant open sem for reading.");
	}

	sem_w=sem_open(sem_name_write, O_WRONLY|O_CREAT, 0660, 1);

	if(sem_w < 0) {
		error("sem_open(): cant open sem for writing.");
	}

	while(1) {
		msg=mmap(0, MSG_MAX_LEN, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
		if(msg==MAP_FAILED) {
			error("map failed");
		}

		if(sem_wait(sem_r) < 0) {
			error("sem error");
		}

		if(strlen(msg) == 0) {
			break;
		}

		write(1, msg, strlen(msg+1));
		printf("\n");

		if(sem_post(sem_w) < 0) {
			error("sem_post(): error");
		}
	}

	if(sem_post(sem_w) < 0) {
		error("sem_post(): error");
	}

	munmap(msg, MSG_MAX_LEN);
	close(shm_fd);
	sem_close(sem_w);
	sem_close(sem_r);
	sem_unlink(sem_name_read);
	sem_unlink(sem_name_write);
	shm_unlink(shm_name);

	return 0;
}
