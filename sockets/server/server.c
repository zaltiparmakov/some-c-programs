#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define BUFFER 250

int main(int argc, char* argv[]) {
	struct sockaddr_in serv_addr, cli_addr;
	int sockfd;
	char c;
	char *buff=(char *) malloc(BUFFER);
	char *dest_ip="0.0.0.0";
	int portno=80;
	extern int optind;
	extern char *optarg;

	while((c=getopt(argc, argv, "a:p:")) != -1) {
		switch(c) {
			case 'a':
				dest_ip=optarg;
				break;
			case 'p':
				portno=atoi(optarg);
				break;
		}
	}

	printf("Listening on: %s:%d\n", dest_ip, portno);
	fflush(stdout);

	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(portno);
	inet_aton(dest_ip, (struct in_addr *) &serv_addr.sin_addr.s_addr);

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0)
		perror("server: cant open socket.");

	bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	while(1) {
		recvfrom(sockfd, buff, BUFFER, 0, (struct sockaddr *) &cli_addr, (socklen_t *) sizeof(&cli_addr));

		if(strstr(buff, "EXIT")) {
			memset(&buff[strlen(buff)-5], 0, 5);
			printf("%s", buff);
			printf("\n");
			break;
		}

		printf("%s\n", buff);
	}

	free(buff);

	return 0;
}
