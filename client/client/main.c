#include "pigeon.h"
#include "command.h"

int main(int argc, char **argv) {

    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        perror("usage : tcpcli < IPaddress>");
        exit(-1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);

	return 0;
}