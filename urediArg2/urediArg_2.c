/*
* Author: Zoran Altiparmakov
* Date: 20.03.2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int g_argc;
char** g_argv;
bool asc = false;
bool dsc = false;

// prototype
void sort();
void swap(char** t1, char** t2);
void printHelp();

int main(int argc, char** argv) {
	g_argc = argc;
	g_argv = argv;
	
	char* options = "adh";

	extern char* optarg;
	extern int optind, optopt;
	
	char c;
	while ((c = getopt(argc, argv, options)) != -1) {
		switch (c) {
		case 'a':
			asc = true;
            sort();
			break;
		case 'd':
			dsc = true;
			sort();
			break;
		case 'h':
			printHelp();
			break;
		case '?':
			printf("Wrong optional argument: %c\n", optopt);
			break;
		}
	}

	exit(0);
}

void printHelp() {
	printf("\nurediArg <parametri> <arg1> <arg2> <arg3> ...\n"
		"Uredi vrstni red in izpiÅ¡e podane argumente. S parametri nadzira naÄin urejanja.\n"
		"Veljavni parametri:\n"
		"-a\nUredi v naraÅ¡ÄajoÄem vrstnem redu.\n"
		"-d\nUredi v padajoÄem vrstnem redu.\n"
		"-h\nIzpiÅ¡e to pomoÄ.\n"
	);
}

void sort() {
	int i=0, optargs=0;

	for(;i<g_argc;i++) {
		if(g_argv[i][0] != '-') {
			optargs++;
		}
	}

	char** argsArray = (char**) calloc(optargs, sizeof(char*));

	int count=0;
	for (i = 1; i<g_argc; i++) {
		if (g_argv[i][0] != '-') {
			argsArray[count++] = g_argv[i];
		}
		g_argv[i] = "0";
	}
	printf("\n--\n");


	int j;
	// BUBBLE SORT	
	for (i = 0; i<count; i++) {
		for (j = 0; j<count - 1; j++) {
			if (asc) {
				if (strcmp(argsArray[j], argsArray[j + 1]) > 0) {
					swap(&argsArray[j], &argsArray[j + 1]);
				}
			}
			else if (dsc) {
				if (strcmp(argsArray[j], argsArray[j + 1]) < 0) {
					swap(&argsArray[j], &argsArray[j + 1]);
				}
			}
		}
	}
	
	
	printf("Sorted: ");
	for (i = 0; i<count; i++) {
		printf("%s ", argsArray[i]);
	}
	
	printf("\n");

	free(argsArray);
}

void swap(char** t1, char** t2) {
	char* temp;
	temp = *t1;
	*t1 = *t2;
	*t2 = temp;
}

