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
    uint8_t         sin_len;
    sa_family_t     sin_family;
    in_port_t       sin_port;
    struct in_addr  sin_addr;
    char            sin_zero[8];
};

struct sockaddr {
    uint8_t         sa_len;
    sa_family_t     sa_family;
    char            sa_data[14];
};

int inet_aton(const char *strptr, struct in_addr *addrptr);
in_addr_t inet_addr(const char *strptr);
char *inet_ntoa(struct in_addr iaddr);

int inet_pton(int family, const char *strptr, void *addrptr);
const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len);

int bind(int, struct sockaddr *, socklen_t);
int accept(int, struct sockaddr *, socklen_t *);
int connect(sockfd, (struct sockaddr *), socklen_t);

int socket(int family, int type, int protocol);

int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
/**
the "op" has some operations
EPOLL_CTL_ADD	add new fd to epfd
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


#endif /** __INCLUDE_DEFINITIONS_H__ **/
