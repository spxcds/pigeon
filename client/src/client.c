#include "pigeon.h"
#include "utils.h"

int main(int argc, char **argv) {

    int sockfd;
    struct sockaddr_in servaddr;

    /**
    if (argc != 2) {
        perror("usage : tcpcli < IPaddress>");
        exit(-1);
    }**/

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    const char *arg = "127.0.0.1";
    inet_pton(AF_INET, arg, &servaddr.sin_addr);

    int confd = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    for (;;) {
        printf("servport = %d, confd = %d, sockfd = %d, I'm in client\n", SERV_PORT, confd, sockfd);
        sleep(1);
    }
//    str_cli(stdin, sockfd);

	return 0;
}