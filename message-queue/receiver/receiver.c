#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <getopt.h>

#define MAX_MSGSIZE 4096 // 4KB
#define MAX_MSG 5

void error(char* msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[]) {
	int qfd;
	struct mq_attr queue;

	queue.mq_msgsize=MAX_MSGSIZE;
	queue.mq_maxmsg=MAX_MSG;

	char c;
	char* queue_name;
	while((c=getopt(argc, argv, "q:")) != -1) {
		switch(c) {
			case 'q':
				queue_name=optarg;
				break;
			case '?':
				fprintf(stderr, "Invalid argument");
				break;
		}
	}

	qfd=mq_open(queue_name, O_RDWR|O_CREAT, 0660, &queue);

	if(qfd < 0)
		error("Opozorilo: vrsta za sporoÄila ne obstaja!");

	char* msg=malloc(MAX_MSGSIZE+1);
	memset(msg, 0, MAX_MSGSIZE+1);

	while(1) {
		mq_receive(qfd, msg, MAX_MSGSIZE, NULL);
		
		if(strlen(msg) == 0) {
			mq_close(qfd);
			mq_unlink(queue_name);
			break;
		} else {
			printf("New message: %s\n", msg);
			fflush(stdout);
		}
		memset(msg,0,MAX_MSGSIZE+1);
	}
	
	return 0;
}
