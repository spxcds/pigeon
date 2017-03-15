#include "pigeon.h"
#include "utils.h"
#include "server.h"

int NewSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, NULL, sizeof(int));
    if (sockfd <= 0) {
        err_quit("NewSocket failed!");
    }
    return sockfd;
}

int ServerInit(struct sockaddr_in *sa, in_port_t port) {
    int listenfd = NewSocket();
    memset(sa, 0, sizeof(*sa));
    sa->sin_family = AF_INET;
    sa->sin_port = htons(port);
    sa->sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)sa, sizeof(*sa)) < 0) {
        err_quit("Server bind failed!");
    }

    if (listen(listenfd, LISTEN_QUEUE_SIZE) < 0) {
        err_quit("Server listen failed!");
    }
    return listenfd;
}

void process(int fdClient) {
/*
    printf("fdClient = %d\n", fdClient);
    char buf[2048];
    int bufLen;
    enum MessageType mt;
    readMsg(fdClient, &mt, buf, &bufLen);
    filehead_t fileHead;
    memcpy(&fileHead, buf, bufLen);
    printf("filename = %s\n", fileHead.fileName);
    printf("filesize = %ld\n", fileHead.fileSize);
    
    /**
    readFileHeadFromBuf(&fileHead, buf, 2048);
    printf("fileHead->name = %s\n", fileHead.fileName);
    printf("fileHead->size = %d\n", fileHead.fileSize);
    **/
}