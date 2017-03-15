#include "pigeon.h"
#include "utils.h"
#include "work.h"

static int CreateFile(int sockfd) {
	enum MessageType mt;
	char buf[BUFFSIZE * 2];
	int bufLen;
	ReadMsg(sockfd, &mt, buf, &bufLen);

    filehead_t fileHead;
    memcpy(&fileHead, buf, bufLen);
	int fd;
	if ((fd = creat(fileHead.fileName, 0777)) < 0) {
		return -1;
	}
	return fd;
}

int RecvFile(int sockfd) {
	int fd;
	fd = CreateFile(sockfd);


	char buf[BUFFSIZE * 2];
	int bufLen;
	enum MessageType mt;
	int cnt = 0;

	while (1) {
		fileblock_t *fileBlock = NULL;
		int len = ReadMsg(sockfd, &mt, buf, &bufLen);
		if (len == -1) {
			break;
		}
		fileBlock = (fileblock_t *)malloc(bufLen);
		memcpy(fileBlock, buf, bufLen);
		printf("bufLen = %d\n", bufLen);
		write(fd, fileBlock->buf, fileBlock->len);
	}


	puts("done!");
	
	close(fd);
	return 0;
}