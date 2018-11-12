/*
1. Write program binarytoascii, which translates binary input to ASCII characters. Program select data from standard input,
and print the results on standard output. Data is written as 8-bit numbers - char type.

Using following flags:
  -o  print results to the file
  -i  get data from the file
  -O  print data on handler n
  -I  get data from handler n

The program must be able to read endless stream of data. We can do that by reading data in small pieces in a while loop, 
until read function does not return that we have reached the end.
*/

/*
 * Date: 25.03.2017
 * Author: Zoran Altiparmakov
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

int inhandler=0;
int outhandler=1;
bool ifile=false;
bool ofile=false;
bool ihandler=false;
bool ohandler=false;
bool stdinput=false;
bool character=false;
	
int main(int argc, char* argv[]) {
	char* fname;
	char* flags="o:i:O:I:c";
	
	extern char* optarg;
	extern int optind, optopt;
	
	char c;
	while((c=getopt(argc, argv, flags)) != -1) {
		switch(c) {
			case 'o':
				ofile=true;
				fname=optarg;
				break;
			case 'i':
				ifile=true;
				fname=optarg;
				break;
			case 'O':
				ohandler=true;
				outhandler=atoi(optarg);
				break;
			case 'I':
				ihandler=true;
				inhandler=atoi(optarg);
				break;
			case 'c':
				character=true;
				break;
			case '?':
				printf("Wrong flag %d\n", optind);
				break;
			case ':':
				printf("Wrong option %s\n", optarg);
				break;
		}
	}
	
	if(optind==1) {
		stdinput=true;
	}
	
	// using o flag: ./binarytoascii -o data_byte.ascii 
	if(ofile) {
		FILE* f=fopen(fname, "w+");

		if(f==NULL) {
			perror("Failed opening the file");
			return -1;
		}
		
		int nread;
		int c;
		char ch;
		while((nread=read(0, &ch, 1)) >0) {
			// if using -c flag, copy as characters
			if(character) {
				fprintf(f, "%c ", c);
			} else {
				c=(int)ch;
				fprintf(f, "%d ", c);
			}
		}
	}
	
	// using i flag: ./binarytoascii -i data_byte.bin
	if(ifile) {
		FILE* f=fopen(fname, "rb");
		
		if(f==NULL) {
			perror("Failed opening the file.");
			return -1;
		}
		
		signed char ch;
		int b,c;
		while((b=fscanf(f,"%c",&ch)) != EOF) {
			c=(int)ch;
			printf("%d ", c);
		}
		
		fclose(f);
	}
	
	if(ohandler) {
		int c;
		unsigned char ch;
		int nb;
		char p[2];

		if(ihandler) {
			while((nb=read(inhandler, &ch, 1))>0) {
				char c;
				c=(int)ch;
				sprintf(p, "%d ", c);
				write(outhandler, p, sizeof(int));
			}
			ihandler=false;
		}
		else {
			while((nb=read(0, &ch, 1))>0) {
				c=(int)ch;
				write(outhandler, &c, 1);
			}
		}
		
		close(inhandler);
		close(outhandler);
	}
	
	// using I flag: ./binarytoascii -I 4 4
	if(ihandler) {
		char ch;
		int c;
		int nb;
		
		while((nb=read(inhandler, &ch, 1))>0) {
			c=(int)ch;
			printf("%d ", c);
		}
		
		close(inhandler);
	}
	
	
	//from standard input
	if(stdinput) {
		char ch;
		int c;
		int nb;
		
		while((nb=read(0, &ch, 1))>0) {
			c=(int)ch;
			printf("%d ",c);
		}
		printf("\n");
	}
	
	return 0;
}

