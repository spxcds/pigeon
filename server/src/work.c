#include "pigeon.h"
#include "utils.h"
#include "work.h"

int CreateFile(const char *fileName) {
	int fd;
	if ((fd = creat(fileName, 0777)) < 0) {
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