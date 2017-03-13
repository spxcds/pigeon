#include "pigeon.h"
#include "utils.h"
#include "work.h"

// server
int main(int argc, char **argv) {
    printf("I'm a good boy!!!\n");

//    daemonize(argv[0]);
    char str[] = "Hello, World";
    err_msg("%s == %s--", str, str);  
    puts("111");
    while (1) {
        err_msg("%s", str);
        sleep(1);
    }
/**
    tpool_t *threadPool = ThpoolInit(4);
    struct sockaddr_in servaddr;
    int listenfd = ServerInit(&servaddr, SERV_PORT);
    static struct epoll_event epollEvent, epollEvents[EPOLL_SIZE];

    int epfd = epoll_create(EPOLL_SIZE);
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = listenfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &epollEvent);
 

    for (;;) {
    	int event_count = epoll_wait(epfd, epollEvents, EPOLL_SIZE, -1);
    	for (int i = 0; i < event_count; ++i) {
    		if (epollEvents[i].data.fd == listenfd) {
    			struct sockaddr_in sockClient;
    			socklen_t clientLen;
    			int fdClient = accept(listenfd, 
    				(struct sockaddr*)&sockClient, &clientLen);
                printf("fdClient = %d\n", fdClient);
                RecvFile("test.txt", fdClient);
    		} else {
    			err_quit("error in epoll_wait");
    		}
    	}
    	sleep(1);
    }
    **/
    return 0;
}

/**

 for( ; ; )
    {
        nfds = epoll_wait(epfd,events,20,500);
        for(i=0;i<nfds;++i)
        {
            if(events[i].data.fd==listenfd) //有新的连接
            {
                connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen); //accept这个连接
                ev.data.fd=connfd;
                ev.events=EPOLLIN|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //将新的fd添加到epoll的监听队列中
            }

            else if( events[i].events&EPOLLIN ) //接收到数据，读socket
            {
                n = read(sockfd, line, MAXLINE)) < 0    //读
                ev.data.ptr = md;     //md为自定义类型，添加数据
                ev.events=EPOLLOUT|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);//修改标识符，等待下一个循环时发送数据，异步处理的精髓
            }
            else if(events[i].events&EPOLLOUT) //有数据待发送，写socket
            {
                struct myepoll_data* md = (myepoll_data*)events[i].data.ptr;    //取数据
                sockfd = md->fd;
                send( sockfd, md->ptr, strlen((char*)md->ptr), 0 );        //发送数据
                ev.data.fd=sockfd;
                ev.events=EPOLLIN|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改标识符，等待下一个循环时接收数据
            }
            else
            {
                //其他的处理
            }
        }
    }
**/
