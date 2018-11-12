#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <getopt.h>

int main(int argc, char** argv) {
	int stdin_pipe[2];
	int stdout_pipe[2];
	
	if(pipe(stdin_pipe) == -1)
		perror("cant fork");
		
	if(pipe(stdout_pipe) == -1)
		perror("cant fork");
	
	int pid;
	int buf=0;	
	pid=fork();

	char* prog_name="";
	
	int c;
	char* options="Urlx";
	int s_toupper=0, s_tolower=0, charToNum=0, deleteRows=0;
	
	while((c=getopt(argc, argv, options)) != -1) {
		switch(c) {
			case 'U':
				s_toupper=1;
				break;
			case 'l':
				s_tolower=1;
				break;
			case 'x':
				charToNum=1;
				break;
			case 'r':
				deleteRows=1;
				break;
			case '?':
				printf("Invalid argument");
				break;
		}
	}

	for(int i=optind; i<argc; i++) {
		if(argv[i][0] != '-')
			prog_name=argv[i];
	}
	
	switch(pid) {
		case -1:
			perror("Cant open");
			break;
		case 0:
			close(stdin_pipe[1]);
			close(stdout_pipe[0]);
			dup2(stdin_pipe[0],0);
			dup2(stdout_pipe[1],1);
			execlp(prog_name,prog_name,NULL);
			fprintf(stderr, "Command not found\n");
			exit(1);
			break;
		default:
			close(stdin_pipe[0]);
			close(stdout_pipe[1]);
			wait(0);
						
			while(read(stdout_pipe[0],&buf,1) > 0) {
				if(deleteRows)
					if(buf=='\n')
						buf=' ';
				if(s_toupper) {
					if(charToNum) {
						if(isprint(buf) == 0)
							printf("%d", buf);
						else
							printf("%c",toupper(buf));
					} else {
						printf("%c",toupper(buf));
					}
				}
				if(s_tolower) {
					if(charToNum) {
						if(isprint(buf) == 0)
							printf("%d", buf);
						else
							printf("%c",tolower(buf));
					} else {
						printf("%c",tolower(buf));
					}
				}
			}
			printf("\n");
			
			close(stdin_pipe[1]);
			close(stdout_pipe[1]);
			exit(0);
		}

	return 0;
}
