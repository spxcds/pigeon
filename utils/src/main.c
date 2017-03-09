#include <stdio.h>
#include "thpool.h"

void task1(){
    printf("Thread #%u working on task1\n", (int)pthread_self());
}


void task2(){
    printf("Thread #%u working on task2\n", (int)pthread_self());
}


int main(){
    
    puts("Making threadpool with 4 threads");
    tpool_t *thpool = ThpoolInit(4);

    puts("Adding 40 tasks to threadpool");
    int i;
    for (i=0; i<20; i++){
        printf("i = %d\n", i);
        ThpoolAddJob(thpool, (void*)task1, NULL);
        ThpoolAddJob(thpool, (void*)task2, NULL);
    };
    ThpoolWait(thpool);
    puts("Killing threadpool");
    ThpoolDestroy(thpool);
    
    return 0;
}