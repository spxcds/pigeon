#include "pigeon.h"
#include "daemonize.h"

#include "work.h"

// server
int main() {
//    daemonize("pigeon");

    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    ServerInit(&servaddr, 1000);
    return 0;
}
