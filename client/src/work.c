#include "pigeon.h"
#include "utils.h"
#include "work.h"

static int SendFileInfo(const char *fileName, int sockfd) {
    char buf[BUFFSIZE * 2];
    filehead_t fileHead;
    int fd = open(fileName, O_RDONLY);
    fileHead.fileSize = lseek(fd, 0, SEEK_END);
    close(fd);

    memcpy(fileHead.fileName, fileName, strlen(fileName));
    memcpy(buf, &fileHead, sizeof(fileHead));
    enum MessageType mt = CREATE_FILE;
    int length = WriteMsg(sockfd, mt, buf, sizeof(fileHead));
    if (length == -1) {
        return -1;
    }
    return 0;
}

int SendFile(const char *fileName, int sockfd) {
    if (SendFileInfo(fileName, sockfd) == -1) {
        err_quit("%s: send file info failed", __FUNCTION__);
    }
    
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        err_quit("%s: open file failed", __FUNCTION__);
    }


    fileblock_t *fileBlock = (fileblock_t *)malloc(BUFFSIZE * sizeof(char));
    if (fileBlock == NULL) {
        err_quit("%s: malloc fileBlock struct failed", __FUNCTION__);
    }

    memcpy(fileBlock->fileName, fileName, strlen(fileName));
    fileBlock->offset = 0;
    char buf[sizeof(fileblock_t) + BUFFSIZE * sizeof(char)];
    

    enum MessageType mt = FILE_BLOCK;
    int len;
    ReadMsg(sockfd, &mt, buf, &len);
    while ((fileBlock->len = read(fd, fileBlock->buf, 
                    BUFFSIZE * sizeof(char) - sizeof(fileblock_t))) > 0) {
//        sleep(1);
        fileBlock->offset = lseek(fd, 0, SEEK_CUR) - fileBlock->len;
/*
        printf("fileBlock->len      = %ld\n", fileBlock->len);
        printf("fileBlock->offset   = %ld\n", fileBlock->offset);
*/
        memcpy(buf, fileBlock, sizeof(fileblock_t) + fileBlock->len);
        int length = WriteMsg(sockfd, mt, buf, sizeof(fileblock_t) + fileBlock->len);

        if (length == -1) {
            err_quit("%s: send file block failed", __FUNCTION__);
        }
        
        ReadMsg(sockfd, &mt, buf, &len);
    }
    mt = FINISHED;
    WriteMsg(sockfd, mt, buf, 0);
    while (1) {
        int len;
        ReadMsg(sockfd, &mt, buf, &len);
        if (mt == FINISHED) {
            break;
        } else {
            puts("mt != success");
        }
        sleep(1);
    }

    return 0;
}