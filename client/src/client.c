#include "pigeon.h"
#include "utils.h"
#include "client.h"

int NewSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, NULL, sizeof(int));
    if (sockfd <= 0) {
        err_quit("NewSocket failed!");
    }
    return sockfd;
}

struct sockaddr_in *NewServerAddr(int sockfd, const char *ip) {
    struct sockaddr_in *servaddr = NULL;
    servaddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    if (servaddr == NULL) {
        err_quit("%s: new server addr failed", __FUNCTION__);
    }
    memset(servaddr, 0, sizeof(struct sockaddr_in));
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, ip, &servaddr->sin_addr);
    return servaddr;
}

int BuildConnection(const char *ip, int *fdArray, int len) {
    for (int i = 0; i < len; ++i) {
        int sockfd = NewSocket();
        struct sockaddr_in *servaddr = NewServerAddr(sockfd, ip);

        if (connect(sockfd, (struct sockaddr *)servaddr, 
                                    sizeof(*servaddr)) != 0) {
            err_quit("connect failed");
        }
        fdArray[i] = sockfd;
    }
    return 0;
}


int FdsetInit(fdset_t *fdSet) {
    if (fdSet == NULL) {
        return -1;
    }

    for (int i = 0; i < THREADNUM; ++i) {
        sem_init(&fdSet->isFree[i], 0, 1);    
    }
    
    return 0;
}

int FdsetDestroy(fdset_t *fdSet) {
    if (fdSet == NULL) {
        return -1;
    }

    for (int i = 0; i < THREADNUM; ++i) {
        close(fdSet->sockfdArray[i]);
        close(fdSet->filefdArray[i]);
        sem_destroy(&fdSet->isFree[i]);
    }

    return 0;
}