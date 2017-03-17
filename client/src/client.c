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
