ï»¿/*
* Author: Zoran Altiparmakov
* Date: 13.06.2017
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <stdint.h>

long *table;
int *indexTab;

pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t con=PTHREAD_COND_INITIALIZER;

int start_flag = 0;

int n;

void *thread_main(void *argv);

int main(int argc, char* argv[])
{
    if(argc<2) {
        n=5;
    }
    else {
        n=atoi(argv[1]);
    }

    table=malloc(n*sizeof(long));
    indexTab=malloc(n*sizeof(int));

    int i;
    pthread_t threads[n];
    
    printf("Ustvarjam %d niti...\n", n);

    pthread_mutex_lock(&mut);

    for(i=0;i<n;i++) {
        indexTab[i] = i;
        pthread_create(&threads[i], NULL, thread_main, (void*)(intptr_t)i);
        printf("Nit %d (TID: %u) se je zagnala\n", i, (int)threads[i]);
        table[i]=threads[i];
    }

        start_flag=1;
        pthread_cond_broadcast(&con);
        pthread_mutex_unlock(&mut);

    pthread_join(table[0], NULL);
    
    free(table);
    free(indexTab);

    return 0;
}

void *thread_main(void *argv) {
    int x=(intptr_t)argv;
    
    pthread_mutex_lock(&mut);

    pthread_mutex_unlock(&mut);

    while(start_flag != 1)
        pthread_cond_wait(&con, &mut);

    if(indexTab[x] != n-1)
        pthread_join(table[x+1], 0);

    printf("nit %d\n", x);

    return 0;
}

