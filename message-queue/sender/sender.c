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

void error(char* msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char* argv[]) {
	int qfd;

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

	qfd=mq_open(queue_name, O_WRONLY);

	if(qfd == -1)
		error("Opozorilo: vrsta za sporoÄila ne obstaja!");

	char* msg=malloc(MAX_MSGSIZE);

	memset(msg, 0, MAX_MSGSIZE);

	for(int i=optind; i<argc; i++) {
		strcat(msg, argv[i]);
		strcat(msg, " ");
	}

	if(mq_send(qfd, msg, strlen(msg), 0) == -1)
		error("mq_send");
		
	printf("%s", msg);

	return 0;
}
