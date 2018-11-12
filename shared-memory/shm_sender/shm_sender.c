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

	shm_fd=shm_open(shm_name, O_RDWR, 0);

	if(shm_fd < 0) {
		fprintf(stderr, "Opozorilo: pomnilnik in semaforji ne obstajajo!");
		return 0;
	}

	sem_r=sem_open(sem_name_read, O_RDONLY);

	if(sem_r < 0) {
		error("sem_open(): cant open sem for reading.");
	}

	sem_w=sem_open(sem_name_write, O_WRONLY);

	if(sem_w < 0) {
		error("sem_open(): cant open sem for writing.");
	}

	if(sem_wait(sem_w) < 0) {
		error("sem_wait(): error waiting.");
	}

	char arguments[MSG_MAX_LEN];

	int i;
	for(i=optind; i<argc; i++) {
		strcat(arguments, argv[i]);
		strcat(arguments, " ");
	}

	msg=mmap(0, MSG_MAX_LEN, PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if(msg==MAP_FAILED) {
		error("map failed");
	}

	strcpy(msg, arguments);

	if(sem_post(sem_r) < 0) {
		error("sem_post(): error");
	}

	return 0;
}
