#ifndef __INCLUDE_THPOLL_H__
#define __INCLUDE_THPOLL_H__

#include "pigeon.h"

/* job */
typedef struct job{
    void            (*function)(void*);
    void            *arg;
    struct job      *next;
} job_t;
    

/* job queue */
typedef struct jobqueue{
    pthread_mutex_t     jobMutex;
    job_t               *front;
    job_t               *rear;
    sem_t               hasJob;
    int                 jobsNum;
} jobqueue_t;


/* thread */
typedef struct tpool tpool_t;
typedef struct thread{
    int                 id;
    pthread_t           pthread;
    tpool_t             *thpool;
} thread_t;


/* threadpool */
typedef struct tpool{
    thread_t            **threads;
    volatile int        numThreadsAlive;
    volatile int        numThreadsWorking;
    volatile int        keepAlive;
    volatile int        onHold;
    pthread_mutex_t     tpoolMutex;
    pthread_cond_t      threadsAllIdle;           /* signal to tpool_wait */
    jobqueue_t          jobqueue;
} tpool_t;

/* initialize the thread pool */
tpool_t *ThpoolInit(int num_threads);

/*  add work to the job pool */
int ThpoolAddJob(tpool_t *, void (*function_p)(void*), void* arg_p);

/*  wait for all queued jobs to finish */
void ThpoolWait(tpool_t *);

/*  pause all threads immediately */
void ThpoolPause(tpool_t *);

/*  continue all threads if they are paused */
void ThpoolResume(tpool_t *);

/*  destroy the pthpool */
void ThpoolDestroy(tpool_t *);

/*  show currently working threads */
int ThpoolWorkingNum(tpool_t *);

/*  show currently alive threads */
int ThpoolAliveNum(tpool_t *);

/*  show currently size of jobs */
int ThpoolJobsNum(tpool_t *);


#endif  /** __INCLUDE_THPOLL_H__ **/