#include "work.h"

int NewSocket() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

int CreateFile(const char *fileName) {
	int fd;
	if ((fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC)) <= 0) {
		return -1;
	}
	return fd;
}

int RecvFile(const char *fileName, int sockfd) {
	int fd;
	if ((fd = CreateFile(fileName)) < 0) {
		err_sys("create file failed");
	}

	ssize_t len;
	char buf[200];
	printf("before read...\n");

	while ((len = read(sockfd, buf, 16)) > 0) {
		printf("buf = %s\n", buf);
		printf("fd = %d\n", fd);
		if (write(fd, buf, len) != len) {
			err_sys("write error!");
		}
	}

	printf("len = %ld\n", len);
	printf("after read...\n");
	close(fd);
	
	return 0;
}