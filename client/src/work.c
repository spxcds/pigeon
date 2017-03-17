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

    if (WriteMsg(sockfd, mt, buf, sizeof(fileHead)) == -1) {
        err_quit("%s: send message error!");
        return -1;
    }
    return 0;
}

static void *SendFileBlock(void *arg_) {
    SendFileBlockArg_t *arg = (SendFileBlockArg_t *)arg_;
    if (arg == NULL) {
        err_msg("%s: arg is NULL", __FUNCTION__);
    }

    sem_wait(&arg->hasResource);
    fileblock_t *fileBlock = (fileblock_t *)malloc(arg->len * sizeof(char));
    if (fileBlock == NULL) {
        err_quit("%s: malloc fileBlock struct failed", __FUNCTION__);
    }
    char namebuf[] = "test.txt";
    memcpy(fileBlock->fileName, namebuf, sizeof(namebuf));
    fileBlock->offset = arg->offset;
    printf("offset = %ld len = %ld\n", fileBlock->offset, fileBlock->len);
    char buf[sizeof(fileblock_t) + arg->len * sizeof(char)];
    lseek(arg->filefd, arg->offset, SEEK_SET);
    enum MessageType mt = FILE_BLOCK;
    fileBlock->len = read(arg->filefd, fileBlock->buf, 
                    arg->len * sizeof(char) - sizeof(fileblock_t));
    memcpy(buf, fileBlock, sizeof(fileblock_t) + fileBlock->len);
    if (WriteMsg(arg->sockfd, mt, buf, sizeof(fileblock_t) + fileBlock->len) == -1) {
        err_quit("%s: send file block failed", __FUNCTION__);
    }
    free(fileBlock);

    sem_post(&arg->hasResource);
}

int SendFile(const char *fileName, fdset_t *fdSet) {
    if (fileName == NULL || fdSet == NULL) {
        err_msg("%s: fileName or fdSet is NULL", __FUNCTION__);
        return -1;
    }
    if (SendFileInfo(fileName, fdSet->sockfdArray[0]) == -1) {
        err_quit("%s: send file info failed", __FUNCTION__);
    }

    int len;
    enum MessageType mt;
    char buf[sizeof(fileblock_t) + BUFFSIZE * sizeof(char)];
    ReadMsg(fdSet->sockfdArray[0], &mt, buf, &len);

    if (mt == SUCCESS) {
        tpool_t *threadPool = ThpoolInit(THREADNUM);
        for (int i = 0; i < THREADNUM; ++i) {
            fdSet->filefdArray[i] = open(fileName, O_RDONLY);
        }

        int fileSize = lseek(fdSet->filefdArray[0], 0, SEEK_END);
        for (int i = 0; i < fileSize; i += BUFFSIZE) {
            SendFileBlockArg_t *arg = 
                        (SendFileBlockArg_t*)malloc(sizeof(SendFileBlockArg_t));
            arg->sockfd = fdSet->sockfdArray[i % THREADNUM];
            arg->filefd = fdSet->filefdArray[i % THREADNUM];
            arg->offset = (size_t)i;
            
            arg->len = BUFFSIZE;
            if (arg->offset + arg->len > fileSize) {
                arg->len = fileSize - arg->offset;
            }
            printf("offset = %ld len = %ld\n", arg->offset, arg->len);
            ThpoolAddJob(threadPool, (void*)SendFileBlock, (void *)arg);
        }
    } else {
        err_msg("%s: receive success message error", __FUNCTION__);
        return -1;
    }

    mt = FINISHED;
    WriteMsg(fdSet->sockfdArray[0], mt, buf, 0);
    printf("send finished \n");
    ReadMsg(fdSet->sockfdArray[0], &mt, buf, &len);    

    if (mt != FINISHED) {
        err_msg("%s: receive finished message error", __FUNCTION__);
        return -1;
    }

    return 0;
}