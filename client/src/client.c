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
    printf("confd = %d\n", confd);
    int fd = open("test.txt", O_RDONLY);

    char buf[200];
    int len = 20;
    int n;

    while ((n = read(fd, buf, len)) > 0) {
        buf[n] = 0;
        printf("buf = %s\n", buf);
        int slen;
        printf("confd = %d\n", confd);
        if ((slen = send(sockfd, buf, n, 0)) <= 0) {
            perror("");
            err_quit("write error");
        }
        printf("slen = %d\n", slen);
        //printf("n = %d\n", n);
    }

    for (;;) {
        printf("servport = %d, confd = %d, sockfd = %d, I'm in client\n", SERV_PORT, confd, sockfd);
        sleep(1);
    }
    close(confd);
//    str_cli(stdin, sockfd);

	return 0;
}