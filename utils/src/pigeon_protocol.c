#include "pigeon.h"
#include "utils.h"
#include "pigeon_protocol.h"

static unsigned short msgCheck(char *buf, int len) {
    if (buf == NULL) {
        return 0;
    }
    unsigned short checkNum = 0;
    for (int i = 0; i < len; ++i) {
        checkNum += buf[i];
    }

    return checkNum;
}

/*
int writeFileHeadToBuf(filehead_t *fileHead, void *buf_, int len) {
    if (fileHead == NULL || buf_ == NULL) {
        return -1;
    }
    char *buf = (char *)buf_;
    int length = 0, tlen;

    tlen = snprintf(buf + length, len - length, "%s", fileHead->fileName);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = snprintf(buf + length, len - length, "%d", fileHead->fileSize);
    length += tlen + 1;
    if (length > len) return -1;
    /*
    tlen = snprintf(buf + length, len - length, "%d", fileHead->blockCount);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = snprintf(buf + length, len - length, "%d", fileHead->blockSize);
    length += tlen + 1;
    if (length > len) return -1;
    
    return length;
}

int readFileHeadFromBuf(filehead_t *fileHead, void *buf_, int len) {
    if (fileHead == NULL || buf_ == NULL) {
        return -1;
    }
    char *buf = (char *)buf_;
    int length = 0, tlen;

    tlen = sscanf(buf + length, "%s", fileHead->fileName);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = sscanf(buf + length, "%d", &fileHead->fileSize);
    length += tlen + 1;
    if (length > len) return -1;
    /*
    tlen = sscanf(buf + length, "%d", &fileHead->blockCount);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = sscanf(buf + length, "%d", &fileHead->blockSize);
    length += tlen + 1;
    if (length > len) return -1;
   
    return length;
}
**/
/**
static int writeFileBlockToBuf(fileblock_t *fileBlock, void *buf_, int len) {
    if (fileBlock == NULL || buf_ == NULL) {
        return -1;
    }
    char *buf = (char *)buf_;
    int length = 0, tlen;

    tlen = snprintf(buf + length, len - length, "%s", fileBlock->fileName);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = snprintf(buf + length, len - length, "%ld", fileBlock->offset);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = snprintf(buf + length, len - length, "%ld", fileBlock->len);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = snprintf(buf + length, len - length, "%s", fileBlock->content);
    length += tlen + 1;
    if (length > len) return -1;
    
    return 0;
}


static int readFileBlockFromBuf(fileblock_t *fileBlock, void *buf_, int len) {
    if (fileBlock == NULL || buf_ == NULL) {
        return -1;
    }
    char *buf = (char *)buf_;
    int length = 0, tlen;

    tlen = sscanf(buf + length, "%s", fileBlock->fileName);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = sscanf(buf + length, "%ld", &fileBlock->offset);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = sscanf(buf + length, "%ld", &fileBlock->len);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = sscanf(buf + length, "%s", fileBlock->content);
    length += tlen + 1;
    if (length > len) return -1;
    
    return 0;
}


filehead_t *createFileHead() {
    filehead_t *pfileHead = (filehead_t *)malloc(sizeof(filehead_t));

    if (pfileHead == NULL) {
        return NULL;
    } else {
        pfileHead->fileName;
        pfileHead->fileSize;
        blockCount;
        blockSize 
    }

    return pfileHead;
}
**/
static msg_t *NewMsg(size_t bufLen) {
    msg_t *msg = (msg_t *)malloc(sizeof(msg_t) + bufLen);
    return msg;
}


/* write the msg */
int WriteMsg(int sockfd, enum MessageType mt, 
                        void *buf, int bufLen) {
    //puts("----------------------------");
    msg_t *msg = NewMsg(bufLen);
    if (msg == NULL) {
        err_msg("%s: new message failed", __FUNCTION__);
        return -1;
    }
    msg->type = mt;
    msg->bufLen = bufLen;
    memcpy(msg->buf, buf, bufLen);
    msg->checkNum = msgCheck(msg->buf, msg->bufLen);
    // printf("msg->checkNum = %u msg->bufLen = %ld\n", msg->checkNum, msg->bufLen);
    
    if (send(sockfd, msg, sizeof(msg_t) + msg->bufLen, 0) != 
                        sizeof(msg_t) + msg->bufLen) {
        err_msg("%s: send error!", __FUNCTION__);
        return -1;
    }
    
    free(msg);
    return 0;
}

/* read the msg */
int ReadMsg(int sockfd, enum MessageType *mt,
                        void *buf, int *bufLen) {
    //puts("--------------------------------");
    msg_t *msg = NewMsg(0);
    if (msg == NULL) {
        err_msg("%s: new message failed", __FUNCTION__);
        return -1;
    }

    int recvHeadNum = 0;
    msg->type = 1111;
    msg->checkNum = 1111;
    msg->bufLen = 1111;
    if ((recvHeadNum = recv(sockfd, msg, sizeof(msg_t), 0)) < 0) {
        err_msg("%s: recv message head error!", __FUNCTION__);
        return -1;
    }

    // printf("recv = %d msg->type = %u msg->checkNum = %u msg->bufLen = %ld\n", recvHeadNum, msg->type, msg->checkNum, msg->bufLen);

    if (recvHeadNum == 0) {
        *mt = FINISHED;
        *bufLen = 0;
        free(msg);
        return 0;
    }
    
    int recvLen = 0;
    while (msg->bufLen > 0 && recvLen < msg->bufLen) {
        int oneRecv = 0;
        if ((oneRecv =  recv(sockfd, buf + recvLen, msg->bufLen - recvLen, 0)) < 0) {
            free(msg);
            err_msg("%s: recv message body error!", __FUNCTION__);
            return -1;
        }
        recvLen += oneRecv;
    }

    *mt = msg->type;
    *bufLen = msg->bufLen;

    if (msg->checkNum != msgCheck(buf, msg->bufLen)) {
        err_msg("%s: check failed", __FUNCTION__);
        free(msg);
        return -1;
    }
    
    free(msg);

    return 0;
}