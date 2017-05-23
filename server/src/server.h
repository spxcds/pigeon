#ifndef __SERVER_SERVER_H__
#define __SERVER_SERVER_H__

#define LISTEN_QUEUE_SIZE   20          /** listen queue size **/
#define EPOLL_SIZE          100         /** epoll listened size **/
#define THREADNUM           1

struct epoll_event epollEvent, epollEvents[EPOLL_SIZE];
int epfd;

typedef struct  fdset {
    int         sockfdArray[THREADNUM];
    int         filefdArray[THREADNUM];
    sem_t       isFree[THREADNUM];
}fdset_t;

/** new a socket **/
int NewSocket();

/** initial server socket, with passed struct sockaddr and port **/
int ServerInit(struct sockaddr_in *sa, in_port_t port);

void process(int fd);

int FdsetInit(fdset_t *fdSet);

int FdsetDestroy(fdset_t *fdSet);

#endif /** __SERVER_SERVER_H__ **/