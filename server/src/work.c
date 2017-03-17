#include "pigeon.h"
#include "utils.h"
#include "work.h"
#include "pigeon_protocol.h"

static int CreateFile(const char *buf, int bufLen) {
    filehead_t fileHead;
    memcpy(&fileHead, buf, bufLen);

    int fd;
    if ((fd = creat(fileHead.fileName, 0777)) < 0) {
        return -1;
    }
    close(fd);
    return 0;
}

static int WriteFileBlock(const char *buf, int bufLen) {
    fileblock_t *fileBlock = NULL;
    if ((fileBlock = (fileblock_t *)malloc(bufLen)) == NULL) {
        err_msg("%s: malloc file block struct error", __FUNCTION__);
        return -1;
    }
    memcpy(fileBlock, buf, bufLen);
    int fd;
    if ((fd = open(fileBlock->fileName, O_RDONLY)) == -1) {
        err_msg("%s: open %s error", __FUNCTION__, fileBlock->fileName);
    }
    printf("filename = %s fd = %d offset = %ld\n", fileBlock->fileName, fd, fileBlock->offset);
    printf("len = %ld buf = %s\n", fileBlock->len, fileBlock->buf);
    lseek(fd, fileBlock->offset, SEEK_SET);
    write(fd, fileBlock->buf, fileBlock->len);
    close(fd);
    return 0;
}

void *RecvFile(void *arg_) {
    RecvFileArg_t *arg = (RecvFileArg_t *)arg_;
    enum MessageType mt;
    char buf[BUFFSIZE * 2];
    int bufLen;
    ReadMsg(arg->sockfd, &mt, buf, &bufLen);
    switch (mt) {
        case CREATE_FILE:
            CreateFile(buf, bufLen);
            mt = SUCCESS;
            WriteMsg(arg->sockfd, mt, buf, 0);
            break;
        case SUCCESS:
            break;
        case FILE_BLOCK:
            WriteFileBlock(buf, bufLen);
            break;
        case FAILURE:
            break;
        case FINISHED:
            printf("send FINISHED\n");
            mt = FINISHED;
            WriteMsg(arg->sockfd, mt, buf, 0);
            break;;
        default:
            err_msg("%s: message type error", __FUNCTION__);
    }
}