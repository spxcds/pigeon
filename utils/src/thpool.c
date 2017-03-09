#include "pigeon.h"
#include "error_process.h"
#include "thpool.h"

static int JobqueueInit(jobqueue_t *);
static void JobqueueDestroy(jobqueue_t *);
static job_t *JobqueueFront(jobqueue_t *);
static void JobqueuePush(jobqueue_t*, job_t*);
static int JobqueueIsEmpty(jobqueue_t *);

static int ThreadInit(tpool_t *, thread_t **, int);
static void *ThreadRoutine(thread_t *);


/* initialize thread pool */
tpool_t *ThpoolInit(int num_threads) {
    if (num_threads < 0){
        num_threads = 0;
    }

    /* malloc a new thread pool */
    tpool_t *threadPool = NULL;
    threadPool = (tpool_t *)malloc(sizeof(tpool_t));
    if (threadPool == NULL){
        err_sys("%s: Allocate memory for thread pool failed\n", __FUNCTION__);
        return NULL;
    }
    threadPool->numThreadsAlive = 0;
    threadPool->numThreadsWorking = 0;
    threadPool->onHold = 0;
    threadPool->keepAlive = 1;

    /* initialize the job queue */
    if (JobqueueInit(&threadPool->jobqueue) != 0){
        err_sys("%s: Allocate memory for job queue failed\n", __FUNCTION__);
        free(threadPool);
        return NULL;
    }

    threadPool->threads = (thread_t**)malloc(num_threads * sizeof(struct thread *));
    if (threadPool->threads == NULL){
        err_sys("%s: Allocate memory for threads failed\n", __FUNCTION__);
        JobqueueDestroy(&threadPool->jobqueue);
        free(threadPool);
        return NULL;
    }

    pthread_mutex_init(&(threadPool->tpoolMutex), NULL);
    pthread_cond_init(&threadPool->threadsAllIdle, NULL);

    /* thread init */
    int i;
    for (i = 0; i < num_threads; ++i) {
        ThreadInit(threadPool, &threadPool->threads[i], i);
    }
    
    /* wait for threads to initialize */
    while (threadPool->numThreadsAlive < num_threads) {
    }

    return threadPool;
}


/* add a new job to the thread pool */
int ThpoolAddJob(tpool_t *threadPool, void (*function)(void*), void* args) {
    job_t * newJob = NULL;
    newJob = (job_t *)malloc(sizeof(job_t));

    if (newJob == NULL) {
        err_sys("%s: Allocate memory for new job failed\n", __FUNCTION__);
        return -1;
    }

    newJob->function = function;
    newJob->arg = args;

    JobqueuePush(&threadPool->jobqueue, newJob);

    return 0;
}


/* wait until all jobs have finished */
void ThpoolWait(tpool_t *threadPool){
    pthread_mutex_lock(&threadPool->tpoolMutex);
    while (JobqueueIsEmpty(&threadPool->jobqueue) == 0 
                || threadPool->numThreadsWorking > 0) {
        pthread_cond_wait(&threadPool->threadsAllIdle, &threadPool->tpoolMutex);
    }
    pthread_mutex_unlock(&threadPool->tpoolMutex);
}


/* destroy the threadpool */
void ThpoolDestroy(tpool_t *threadPool){
    if (threadPool == NULL) return ;

    threadPool->keepAlive = 0;

    while (threadPool->numThreadsAlive > 0) {
        sem_post(&threadPool->jobqueue.hasJob);
    }

    JobqueueDestroy(&threadPool->jobqueue);
    
    int i;
    for (i = 0; i < threadPool->numThreadsAlive; ++i) {
        free(threadPool->threads[i]);
    }
    free(threadPool->threads);
    free(threadPool);
}


/**
void thpool_pause(thpool_* thpool_p) {
    int n;
    for (n=0; n < thpool_p->num_threads_alive; n++){
        pthread_kill(thpool_p->threads[n]->pthread, SIGUSR1);
    }
}



void thpool_resume(thpool_* thpool_p) {
    // resuming a single threadpool hasn't been
    // implemented yet, meanwhile this supresses
    // the warnings
    (void)thpool_p;

    threads_on_hold = 0;
}


int thpool_num_threads_working(thpool_* thpool_p){
    return thpool_p->num_threads_working;
}
**/


static int ThreadInit(tpool_t *threadPool, thread_t **threads, int id) {
    *threads = (thread_t *)malloc(sizeof(thread_t));
    if (*threads == NULL){
        err_sys("%s: Allocate memory for thread failed\n", __FUNCTION__);
        return -1;
    }

    (*threads)->id     = id;
    (*threads)->thpool = threadPool;

    if (pthread_create(&(*threads)->pthread, NULL,
                            (void *)ThreadRoutine, (void*)(*threads)) != 0) {
        err_sys("%s: create pthread failed\n", __FUNCTION__);
        return -1;
    }
    pthread_detach((*threads)->pthread);
    return 0;
}


/**
static void thread_hold(int sig_id) {
    (void)sig_id;
    threads_on_hold = 1;
    while (threads_on_hold){
        sleep(1);
    }
}
**/


static void *ThreadRoutine(thread_t *thread) {

    // assure all threads have been created before starting serving 
    tpool_t *threadPool = thread->thpool;

    // Register signal handler
    /**
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = thread_hold;
    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        err("thread_do(): cannot handle SIGUSR1");
    }
    **/

    
    pthread_mutex_lock(&threadPool->tpoolMutex);
    threadPool->numThreadsAlive++;
    pthread_mutex_unlock(&threadPool->tpoolMutex);

    while (threadPool->keepAlive == 1) {
        sem_wait(&(threadPool->jobqueue.hasJob));

        if (threadPool->keepAlive == 1) {
            pthread_mutex_lock(&threadPool->tpoolMutex);
            threadPool->numThreadsWorking++;
            pthread_mutex_unlock(&threadPool->tpoolMutex);

            // read and execute the job from job queue
            void (*func_buff)(void*);
            void    *arg_buff;
            job_t *newJob = JobqueueFront(&threadPool->jobqueue);
            if (newJob != NULL) {
                func_buff = newJob->function;
                arg_buff  = newJob->arg;
                func_buff(arg_buff);
                free(newJob);
            }

            pthread_mutex_lock(&threadPool->tpoolMutex);
            threadPool->numThreadsWorking--;
            if (threadPool->numThreadsWorking == 0) {
                pthread_cond_signal(&threadPool->threadsAllIdle);
            }
            pthread_mutex_unlock(&threadPool->tpoolMutex);

        }
    }
    pthread_mutex_lock(&threadPool->tpoolMutex);
    threadPool->numThreadsAlive--;
    pthread_mutex_unlock(&threadPool->tpoolMutex);

    return NULL;
}


/* initialize queue */
static int JobqueueInit(jobqueue_t *JbQueue){
    JbQueue->front  = NULL;
    JbQueue->rear   = NULL;

    sem_init(&JbQueue->hasJob, 0, 0);
    pthread_mutex_init(&(JbQueue->jobMutex), NULL);

    return 0;
}


/*  destroy the job queue */
static void JobqueueDestroy(jobqueue_t *JbQueue){
    pthread_mutex_lock(&JbQueue->jobMutex);
    while (sem_trywait(&JbQueue->hasJob) == 0) {
        free(JobqueueFront(JbQueue));
    }
    sem_destroy(&JbQueue->hasJob);
    pthread_mutex_unlock(&JbQueue->jobMutex);
}


/*  push a job into the job queue */
static void JobqueuePush(jobqueue_t* JbQueue, job_t* newJob){

    pthread_mutex_lock(&JbQueue->jobMutex);
    newJob->next = NULL;

    if (JbQueue->rear == NULL) {
        JbQueue->front = newJob;
        JbQueue->rear = newJob;
    } else {
        JbQueue->rear->next = newJob;
        JbQueue->rear = newJob;
    }
    sem_post(&JbQueue->hasJob);
    pthread_mutex_unlock(&JbQueue->jobMutex);
}


/*  get one job from queue */
static job_t *JobqueueFront(jobqueue_t *JbQueue) {
    pthread_mutex_lock(&JbQueue->jobMutex);
    job_t* job_p = JbQueue->front;
    if (JbQueue->front != NULL) {
        JbQueue->front = JbQueue->front->next;
        if (JbQueue->front == NULL) {
            JbQueue->rear = NULL;
        }
    }

    pthread_mutex_unlock(&JbQueue->jobMutex);
    return job_p;
}

/* judge the job queue if empty */
static int JobqueueIsEmpty(jobqueue_t *JbQueue) {
    pthread_mutex_lock(&JbQueue->jobMutex);
    int isEmpty = JbQueue->front == NULL;
    pthread_mutex_unlock(&JbQueue->jobMutex);
    return isEmpty;
}