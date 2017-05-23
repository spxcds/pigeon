#include "pigeon.h"
#include "utils.h"
#include "work.h"
#include "server.h"

// server
int main(int argc, char **argv) {
//    daemonize(argv[0]);

    tpool_t *threadPool = ThpoolInit(THREADNUM);
    struct sockaddr_in servaddr;
    int listenfd = ServerInit(&servaddr, SERV_PORT);
    
    epfd = epoll_create(EPOLL_SIZE);
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = listenfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &epollEvent);

    for (;;) {
        int event_count = epoll_wait(epfd, epollEvents, EPOLL_SIZE, -1);
        //printf("event_count = %d\n", event_count);
        for (int i = 0; i < event_count; ++i) {
            if (epollEvents[i].data.fd == listenfd) {
                struct sockaddr_in sockClient;
                socklen_t clientLen = sizeof(struct sockaddr_in);
                int fdClient = accept(listenfd, 
                    (struct sockaddr*)&sockClient, &clientLen);
                if (fdClient == -1) {
                    err_msg("%s: accept error", __FUNCTION__);
                }

                struct epoll_event ev;
                ev.data.fd = fdClient;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, fdClient, &ev);
        
            } else if (epollEvents[i].events & EPOLLIN) {
                // printf("=======i = %d\n fd = %d\n", i, epollEvents[i].data.fd);
                RecvFileArg_t arg;
                arg.sockfd = epollEvents[i].data.fd;
                RecvFile((void*)&arg);
                // ThpoolAddJob(threadPool, (void*)RecvFile, (void*)&arg);
            } else {
                err_quit("error in epoll_wait");
            }
        }
        //sleep(1);
    }

    return 0;
}