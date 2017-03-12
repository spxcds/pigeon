#ifdef	__INCLUDE_DEFINITIONS_H__
//#define __INCLUDE_DEFINITIONS_H__
/**
socketlen_t         uint32_t
in_addr_t           uint32_t
in_port_t           uint16_t
**/

typedef union epoll_data {  
    void *ptr;  
    int fd;  
    __uint32_t u32;  
    __uint64_t u64;  
} epoll_data_t;  

struct epoll_event {  
    __uint32_t events; 		// Epoll events 
    epoll_data_t data; 		// User data variable
};

struct in_addr {
    in_addr_t s_addr;
};

struct sockaddr_in {
//    uint8_t         sin_len;
    sa_family_t     sin_family;
    in_port_t       sin_port;
    struct in_addr  sin_addr;
    char            sin_zero[8];
};

struct sockaddr {
//    uint8_t         sa_len;
    sa_family_t     sa_family;
    char            sa_data[14];
};

int inet_aton(const char *strptr, struct in_addr *addrptr);
in_addr_t inet_addr(const char *strptr);
char *inet_ntoa(struct in_addr iaddr);

int inet_pton(int family, const char *strptr, void *addrptrz);
const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len);

int bind(int, struct sockaddr *, socklen_t);
int accept(int, struct sockaddr *, socklen_t *);
int connect(sockfd, (struct sockaddr *), socklen_t);

ssize_t send(int sockfd, const void *buf, size_t nbytes, int flag);
ssize_t sendto(int sockfd, const void *buf, size_t nbytes, int flag
                const struct sockaddr *destaddr, socklen_t destlen);

ssize_t recv(int sockfd, void *buf, size_t nbytes, int flag);
ssize_t recvfrom(int sockfd, void *buf, size_t nbytes, int flag,
                const struct sockaddr *destaddr, socklen_t destlen););

int socket(int family, int type, int protocol);

int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
/**
the "op" has some operations
EPOLL_CTL_ADD	add a new fd to epfd
EPOLL_CTL_MOD	modify a fd that has been registered
EPOLL_CTL_DEL	delete a fd from epfd

events: 
EPOLLIN 		表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT		表示对应的文件描述符可以写；
EPOLLPRI		表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR		表示对应的文件描述符发生错误；
EPOLLHUP		表示对应的文件描述符被挂断；
EPOLLET 		将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT	只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里

**/

int     pthread_equal(pthread_t, pthread_t);    // judge two threads is if equal
pthread_t pthread_self(void);                   // return the id of the thread


int     pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                        void *(*start_routine) (void *), void *arg);

int     pthread_join(pthread_t thread, void **retval);
int     pthread_cancle(pthread_t);
int     pthread_cancel(pthread_t thread);       // passive stopped
void    pthread_exit(void *retval);             // active stopped 

void pthread_cleanup_push(void (*routine)(void *),
                                 void *arg);

void pthread_cleanup_pop(int execute);
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);

int pthread_attr_getdetachstat(const pthread_t *restrict attr, int *detachstate);
int pthread_attr_setdetachstat(const pthread_t *restrict attr, int  detachstate);
/*
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
*/
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
                            const pthread_mutex_attr_t *mutexattr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_t *mutex);

int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timewait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                            const struct timespec *timeout);

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
/**
1>  unlock(&mutex);
2>  lock(&mutex)
3>  move itself into the wait queue of condition
4>  unlock(&mutex)
5>  wait until other threads wake up itself
6>  lock(&mutex)
7>  delete itself from the wait queue
**/
}

struct timespec {
    time_t tv_sec;  /* seconds */
    long tv_nsec;   /* nanoseconds */ 
}

int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);


int sem_init(sem_t *sem, int pshared, unsigned int value);
/**
pshared = 0, in one process, or, in more than one process
**/
int sem_destroy(sem_t *sem);

int sem_post(sem_t *sem);               //  add one to sem
int sem_wait(sem_t *sem);               //  minus one to sem
int sem_trywait(sem_t *sem);            //  sem_wait non-blocking version


#endif /** __INCLUDE_DEFINITIONS_H__ **/
