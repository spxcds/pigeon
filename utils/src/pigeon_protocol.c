#include "pigeon.h"
#include "utils.h"
#include "pigeon_protocol.h"

static unsigned short msgCheck(msg_t *msg) {
    if (msg == NULL) {
        return 0;
    }
    unsigned short checkNum = 0;

    for (int i = 0; i < msg->bufLen; ++i) {
        checkNum += msg->buf[i];
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
static msg_t *newMsg(size_t bufLen) {
    msg_t *msg = (msg_t *)malloc(sizeof(msg_t) + bufLen);
    return msg;
}


/* write the msg */
int writeMsg(int sockfd, enum MessageType mt, 
                        void *buf, int bufLen) {
    msg_t *msg = newMsg(bufLen);
    if (msg == NULL) {
        err_msg("%s: new message failed", __FUNCTION__);
        return -1;
    }
    msg->type = mt;
    msg->bufLen = bufLen;
    memcpy(msg->buf, buf, bufLen);
    msg->checkNum = msgCheck(msg);

    printf("msg->type = %d\n", msg->type);
    printf("msg->bufLen = %ld\n", msg->bufLen);
    
    if (send(sockfd, msg, sizeof(msg_t) + msg->bufLen, 0) == -1) {
        err_msg("%s: send error!", __FUNCTION__);
        return -1;
    }
    free(msg);
    return 0;
}

/* read the msg */
int readMsg(int sockfd, enum MessageType *mt,
                        void *buf, int *bufLen) {
    msg_t *msg = newMsg(BUFFSIZE);
    if (msg == NULL) {
        err_msg("%s: new message failed", __FUNCTION__);
        return -1;
    }
    int fff = 0;
    if ((fff = recv(sockfd, msg, sizeof(msg_t) + BUFFSIZE, 0)) == -1) {
        err_msg("%s: recv error!", __FUNCTION__);
        return -1;
    }
    
    if (msg->checkNum != msgCheck(msg)) {
        err_msg("%s: check failed", __FUNCTION__);
        return -1;
    }
    
    *mt = msg->type;
    *bufLen = msg->bufLen;
    memcpy(buf, msg->buf, msg->bufLen);
    free(msg);

    return 0;
}