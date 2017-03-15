#include "pigeon.h"
#include "utils.h"
#include "work.h"

int main(int argc, char **argv) {

    int sockfd;
    struct sockaddr_in servaddr;
    
    if (argc != 2) {
        perror("usage: pigeon <local file> <IPaddress>");
        exit(-1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    const char *arg = "127.0.0.1";
    inet_pton(AF_INET, arg, &servaddr.sin_addr);

    int confd = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (confd != 0) {
        err_quit("connect failed");
    }

    SendFile(argv[1], sockfd);
    close(sockfd);
    printf("send success\n");
	return 0;
}