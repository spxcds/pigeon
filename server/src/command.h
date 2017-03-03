#ifndef __COMMAND_H__
#define __COMMAND_H__

#define SERV_PORT 6500
#define LISTENQ   10
#define MAXLINE   512

ssize_t readn(int fd, void *vptr, size_t n) {
    size_t nleft;
    ssize_t nread;

    char *ptr;
    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        } else if (nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return n - nleft;
}

ssize_t writen(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr = vptr;

    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

ssize_t readline(int fd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char c, *ptr;
    ptr = vptr;
    for (n = 1; n < maxlen; ++n) {
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            *ptr = 0;
            return n - 1;
        } else {
            return -1;
        }
    }
    *ptr = 0;
    return n;
}

void str_echo(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        int i = 0; 
        for (i = 0; i < n - 1; ++i) {
            buf[i]++;
        }
        writen(sockfd, buf, n);
    }
    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        perror("str_echo: read error");

}

void str_cli(FILE *fp, int sockfd) {
    int maxfdp1, stdineof = 0;
    fd_set rset;
    char buf[MAXLINE];
    int n;
    FD_ZERO(&rset);
    for (;;) {
        if (stdineof == 0) 
            FD_SET(fileno(fp), &rset);

        FD_SET(sockfd, &rset);
        maxfdp1 = 1 + (fileno(fp) > sockfd ? fileno(fp) : sockfd);
        select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1) {
                    return;
                } else {
                    perror("str_cli: server terminated prematurely");
                    exit(0);
                }
            }
            write(fileno(stdout), buf, n);
        }
        if (FD_ISSET(fileno(fp), &rset)) {
            if ((n = read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            writen(sockfd, buf, n);
        }
    }
}

void sig_chld(int signo) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) 
        printf("child %d terminated!\n", pid);
    return;
}

#endif /** __COMMAND_H__ **/