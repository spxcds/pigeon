#include "pigeon.h"
#include "thpool.h"


static int JobqueueInit(jobqueue_t *);
static void JobqueueDestroy(jobqueue_t *);
static job_t *JobqueueFront(jobqueue_t *);
static void JobqueuePush(jobqueue_t*, job_t*);

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
    threadPool->threadOnHold = 0;
    threadPool->threadsKeepAlive = 1;

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

    pthread_mutex_init(&(threadPool->tpMutex), NULL);
    pthread_cond_init(&threadPool->threadsAllIdle, NULL);

    /* thread init */
    int i;
    for (i = 0; i < num_threads; ++i) {
        ThradInit(threadPool, &threadPool->threads[i], i);
    }
    
    /* wait for threads to initialize */
    while (threadPool->num_threads_alive < num_threads) {
    }

    return threadPool;
}


/* ad a new job to the thread pool */
int ThpoolAddJob(tpool_t *threadPool, void (*function)(void*), void* args) {
    job_t * newJob = NULL
    newJob = (job_t *)malloc(sizeof(job_t));

    if (newJob == NULL) {
        err_sys("%s: Allocate memory for new job failed\n", __FUNCTION__);
        return -1;
    }

    newJob->function=function_p;
    newJob->arg=arg_p;

    JobqueuePush(&threadPool->jobqueue, newJob);

    return 0;
}


/* Wait until all jobs have finished */
void thpool_wait(thpool_* thpool_p){
    pthread_mutex_lock(&thpool_p->thcount_lock);
    while (thpool_p->jobqueue.len || thpool_p->num_threads_working) {
        pthread_cond_wait(&thpool_p->threads_all_idle, &thpool_p->thcount_lock);
    }
    pthread_mutex_unlock(&thpool_p->thcount_lock);
}


/* Destroy the threadpool */
void thpool_destroy(thpool_* thpool_p){
    /* No need to destory if it's NULL */
    if (thpool_p == NULL) return ;

    volatile int threads_total = thpool_p->num_threads_alive;

    /* End each thread 's infinite loop */
    threads_keepalive = 0;

    /* Give one second to kill idle threads */
    double TIMEOUT = 1.0;
    time_t start, end;
    double tpassed = 0.0;
    time (&start);
    while (tpassed < TIMEOUT && thpool_p->num_threads_alive){
        bsem_post_all(thpool_p->jobqueue.has_jobs);
        time (&end);
        tpassed = difftime(end,start);
    }

    /* Poll remaining threads */
    while (thpool_p->num_threads_alive){
        bsem_post_all(thpool_p->jobqueue.has_jobs);
        sleep(1);
    }

    /* Job queue cleanup */
    jobqueue_destroy(&thpool_p->jobqueue);
    /* Deallocs */
    int n;
    for (n=0; n < threads_total; n++){
        thread_destroy(thpool_p->threads[n]);
    }
    free(thpool_p->threads);
    free(thpool_p);
}


/* Pause all threads in threadpool */
void thpool_pause(thpool_* thpool_p) {
    int n;
    for (n=0; n < thpool_p->num_threads_alive; n++){
        pthread_kill(thpool_p->threads[n]->pthread, SIGUSR1);
    }
}


/* Resume all threads in threadpool */
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








static int ThreadInit(tpool_t *threadPool, thread_t **threads, int id) {
    *threads = (thread_t *)malloc(sizeof(thread_t));
    if (*threads == NULL){
        err_sys("%s: Allocate memory for thread failed\n", __FUNCTION__);
        return -1;
    }

    (*threads)->id     = id;
    (*threads)->thpool = threadPool;

    if ((pthread_create(&(*threads)->pthread), NULL,
                            (void *)ThreadRoutine, (void*)(*thread_p)) != 0) {
        err_sys("%s: create pthread failed\n", __FUNCTION__);
        return -1;
    }
    pthread_detach((*threads)->pthread);
    return 0;
}


/* Sets the calling thread on hold */
static void thread_hold(int sig_id) {
    (void)sig_id;
    threads_on_hold = 1;
    while (threads_on_hold){
        sleep(1);
    }
}


static void *ThreadRoutine(thread_t *thread) {

    /* Assure all threads have been created before starting serving */
    thpool_* thpool_p = thread_p->thpool_p;

    /* Register signal handler */
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = thread_hold;
    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        err("thread_do(): cannot handle SIGUSR1");
    }

    /* Mark thread as alive (initialized) */
    pthread_mutex_lock(&thpool_p->thcount_lock);
    thpool_p->num_threads_alive += 1;
    pthread_mutex_unlock(&thpool_p->thcount_lock);

    while(threads_keepalive){

        bsem_wait(thpool_p->jobqueue.has_jobs);

        if (threads_keepalive){

            pthread_mutex_lock(&thpool_p->thcount_lock);
            thpool_p->num_threads_working++;
            pthread_mutex_unlock(&thpool_p->thcount_lock);

            /* Read job from queue and execute it */
            void (*func_buff)(void*);
            void*  arg_buff;
            job* job_p = jobqueue_pull(&thpool_p->jobqueue);
            if (job_p) {
                func_buff = job_p->function;
                arg_buff  = job_p->arg;
                func_buff(arg_buff);
                free(job_p);
            }

            pthread_mutex_lock(&thpool_p->thcount_lock);
            thpool_p->num_threads_working--;
            if (!thpool_p->num_threads_working) {
                pthread_cond_signal(&thpool_p->threads_all_idle);
            }
            pthread_mutex_unlock(&thpool_p->thcount_lock);

        }
    }
    pthread_mutex_lock(&thpool_p->thcount_lock);
    thpool_p->num_threads_alive --;
    pthread_mutex_unlock(&thpool_p->thcount_lock);

    return NULL;
}


/* Frees a thread  */
static void thread_destroy (thread* thread_p){
    free(thread_p);
}



/* initialize queue */
static int JobqueueInit(jobqueue* JbQueue){
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
    newjob->next = NULL;

    if (JbQueue->rear == NULL) {
        JbQueue->head = newJob;
        JbQueue->rear = newJob;
    } else {
        JbQueue->rear->next = newJob;
        JbQueue->rear = newJob;
    }
    sem_post(&JbQueue->hasJob);
    pthread_mutex_lock(&JbQueue->jobMutex);
}


/*  get one job from queue */
static job_t *JobqueueFront(jobqueue_t *JbQueue) {
    pthread_mutex_lock(&JbQueue->jobMutex);
    job_t* job_p = jobqueue_p->front;
    if (JbQueue->front != NULL) {
        JbQueue->front = JbQueue->front->next;
        if (JbQueue->front == NULL) {
            JbQueue->rear = NULL;
        }
    }

    pthread_mutex_unlock(&JbQueue->jobMutex);
    return job_p;
}