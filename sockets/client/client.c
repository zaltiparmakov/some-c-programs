#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER 500

int main(int argc, char* argv[]) {
	struct sockaddr_in ip_addr;
	int sockfd, nbytes;
	char c;
	char *dest_ip="0.0.0.0";
	int portno=0;
	extern int optind;
	extern char *optarg;
	char buff[BUFFER];

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

	memset(&ip_addr, 0, sizeof(struct sockaddr));

	ip_addr.sin_family=AF_INET;
	ip_addr.sin_port=htons(portno);
	inet_aton(dest_ip, (struct in_addr *) &ip_addr.sin_addr.s_addr);

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);

	memset(&buff, 0, BUFFER);
	nbytes=read(0, buff, BUFFER);

	if(nbytes < 0)
		perror("read: error reading");

	nbytes=sendto(sockfd, buff, BUFFER, 0, (const struct sockaddr *) &ip_addr, sizeof(struct sockaddr));
	
	if(nbytes < 0)
		perror("sendto: cant send");

	return 0;
}
