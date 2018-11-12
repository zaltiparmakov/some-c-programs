/*
* Author: Zoran Altiparmakov
* Date: 11.03.2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int g_argc;
char** g_argv;
bool g_nums;

typedef enum {
	ASC, DESC
} order_type;

// prototype
void sort(order_type order, bool num);
void swap(char** t1, char** t2);
int* argvToNum();
void argvToRand();
void printHelp();
void shuffleArgs();

int main(int argc, char* argv[]) {
	g_argc = argc;
	g_argv = argv;
	g_nums = false;
	
	char c;
	extern char* optarg;
	
	char* options = "adnrh";

	extern int optind, optopt;
	int i;
	while ((c = getopt(argc, argv, options)) != -1) {
		switch (c) {
		case 'a':
			for(i=0;i<argc;i++){
				if(strcmp(argv[i], "-n") == 0) {
					g_nums = true;
					break;
				}
			}
			sort(ASC, g_nums);
			break;
		case 'd':
			for(i=0;i<argc;i++){
				if(strcmp(argv[i], "-n") == 0) {
					g_nums = true;
					break;
				}
			}
			sort(DESC, g_nums);
			break;
		case 'r':
			shuffleArgs();
			break;
		case 'h':
			printHelp();
			break;
		case '?':
			printf("Wrong optional argument: %d", c);
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

void sort(order_type order, bool num) {
	int i;
	int count = 0;
	int optargs = 0;
	int* numArray = argvToNum();

	for (i = 0; i<g_argc; i++) {
		if (g_argv[i][0] != '-') {
			optargs++;
		}
	}

	char** argsArray = (char**)malloc(sizeof(char*) * optargs);

	for (i = 1; i<g_argc; i++) {
		if (g_argv[i][0] != '-') {
			argsArray[count++] = g_argv[i];
		}
		g_argv[i] = "0";
	}

	printf("Unsorted: ");
	for (i = 0; i<count; i++) {
		printf("%s ", argsArray[i]);
	}

	printf("\n--\n");


	int j;
	// BUBBLE SORT	
	for (i = 0; i<count; i++) {
		for (j = 0; j<count - 1; j++) {
			if (order == ASC) {
				if (strcmp(argsArray[j], argsArray[j + 1]) > 0 && !num) {
					swap(&argsArray[j], &argsArray[j + 1]);
				} else if(numArray[j] > numArray[j+1]) {
						int temp = numArray[j];
						numArray[j] = numArray[j+1];
						numArray[j+1] = temp;
				}
			}
			else if (order == DESC) {
				if (strcmp(argsArray[j], argsArray[j + 1]) < 0) {
					swap(&argsArray[j], &argsArray[j + 1]);
				} else if(numArray[j] < numArray[j+1]) {
						int temp = numArray[j];
						numArray[j] = numArray[j+1];
						numArray[j+1] = temp;
				}
			}
		}
	}
	
	
	printf("Sorted: ");
	if(!num){
		for (i = 0; i<count; i++) {
			printf("%s ", argsArray[i]);
		}
	} else {
		for(i=0;i<count; i++) {
			printf("%d ", numArray[i]);
		}
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

int* argvToNum() {
	int i;
	int* numArray;
	int count=0;
	int optargs=0;
	
	for(i=1;i<g_argc;i++) {
		if(g_argv[i][0] != '-') {
			optargs++;
		}
	}
	
	numArray = (int*)malloc(sizeof(int*) * optargs);
	
	for(i=1;i<g_argc;i++) {
		if(g_argv[i][0] != '-') {
			numArray[count++] = atoi(g_argv[i]);
		} 
		
		/*
		//check if have integer or float argument and allocate memory
		if(floatNum == (double)atof(g_argv[4])) {
			numArray = (void*)malloc(sizeof(int*) * optargs);
			if(g_argv[i][0] != '-') {
				numArray[count++] = atoi(g_argv[i]);
			}	
		} else {
			numArray = (void*)malloc(sizeof(float*) * optargs);
			if(g_argv[i][0] != '-') {
				numArray[count++] = atof(g_argv[i]);
			}
		}
		* */	
	}
	
	printf("\nSIZE IS: %lu\n", sizeof(numArray));
		
	return numArray;
}

void shuffleArgs() {
	int* numArray = argvToNum();
	int optargs=0;
		
	int i, j;
	
	for(i=1;i<g_argc;i++) {
		if(g_argv[i][0] != '-') {
			optargs++;
		}
	}
	
	printf("Original Array: ");
	for(i=0;i<optargs;i++) {
		printf("%d ", numArray[i]);
	}
	
	puts("");
	
	for(i=1;i<optargs;i++) {
		j = i + rand() / (RAND_MAX / (optargs - i) + 1);
		int t = numArray[j];
		numArray[j] = numArray[i];
		numArray[i] = t;
	}
	
	printf("Shuffled: ");
	for(i=0;i<optargs;i++) {
		printf("%d ", numArray[i]);
	}
}
