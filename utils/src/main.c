#include <stdio.h>
#include "thpool.h"

int cnt;

void task1(int id){
    sleep(1);
    ++cnt;
    printf("Thread #%u working on task1, id = %d\n", (int)pthread_self(), id);
}


void task2(int id){
    sleep(1);
    ++cnt;
    printf("Thread #%u working on task2, id = %d\n", (int)pthread_self(), id);
}


int main(){
    
    puts("Making threadpool with 4 threads");
    tpool_t *thpool = ThpoolInit(4);

    puts("Adding 40 tasks to threadpool");
    int i;
    for (i=0; i<5; i++){
        printf("i = %d\n", i);
        ThpoolAddJob(thpool, (void*)task1, (void*)i);
        ThpoolAddJob(thpool, (void*)task2, (void*)i);
    };
    ThpoolWait(thpool);
    printf("cnt = %d\n", cnt);
    //ThpoolDestroy(thpool);

    sleep(10);
    printf("after killing threadpool\n");
    ThpoolDestroy(thpool);
    while (1) {
        sleep(1);
    }

    return 0;
}