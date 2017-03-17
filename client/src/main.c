#include "pigeon.h"
#include "utils.h"
#include "client.h"
#include "work.h"

int main(int argc, char **argv) {

    if (argc != 2) {
        perror("usage: pigeon <local file> <IPaddress>");
        exit(-1);
    }

    int sockfd = NewSocket(); 
    const char *arg = "127.0.0.1";
    struct sockaddr_in *servaddr = NewServerAddr(sockfd, arg);

    if (connect(sockfd, (struct sockaddr *)servaddr, 
                                sizeof(*servaddr)) != 0) {
        err_quit("connect failed");
    }

    SendFile(argv[1], sockfd);
    close(sockfd);
    printf("send success\n");
    return 0;
}