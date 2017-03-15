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
    mt = SUCCESS;
    WriteMsg(sockfd, mt, buf, 0);
    while (1) {
        fileblock_t *fileBlock = NULL;
        ReadMsg(sockfd, &mt, buf, &bufLen);
        if (mt == FINISHED) {
            close(fd);
            break;
        }
        mt = SUCCESS;
        WriteMsg(sockfd, mt, buf, 0);
//      printf("mt = %d\n", mt);

        fileBlock = (fileblock_t *)malloc(bufLen);
        memcpy(fileBlock, buf, bufLen);
//      printf("bufLen = %d\n", bufLen);
        write(fd, fileBlock->buf, fileBlock->len);
    }

    mt = FINISHED;
    printf("bufore write success");
    WriteMsg(sockfd, mt, buf, 0);

    puts("done!");
    
    close(fd);
    return 0;
}