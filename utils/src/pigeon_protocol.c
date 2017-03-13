#include "pigeon.h"
#include "utils.h"
#include "pigeon_protocol.h"

static unsigned short msgCheck(msg_t *msg) {
    if (msg == NULL) {
        return 0;
    }
    unsigned short checkNum = 0;

    int len = sizeof(msg->buf);
    for (int i = 0; i < len; ++i) {
        checkNum += msg->buf[i];
    }

    return checkNum;
}

static int writeFileHeadToBuf(filehead_t *fileHead, void *buf_, int len) {
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
    
    tlen = snprintf(buf + length, len - length, "%d", fileHead->blockCount);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = snprintf(buf + length, len - length, "%d", fileHead->blockSize);
    length += tlen + 1;
    if (length > len) return -1;
    
    return 0;
}

static int readFileHeadFromBuf(filehead_t *fileHead, void *buf_, int len) {
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
    
    tlen = sscanf(buf + length, "%d", &fileHead->blockCount);
    length += tlen + 1;
    if (length > len) return -1;
    
    tlen = sscanf(buf + length, "%d", &fileHead->blockSize);
    length += tlen + 1;
    if (length > len) return -1;

    return 0;
}

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


/* write the msg */
int writeMsg(int sockfd, enum MessageType mt, 
                        void *buff, int len) {
    msg_t msg;
    memset(&msg, 0, sizeof(msg));

    switch (mt) {
        case SUCCESS:
            break;
        case FAILURE:
            break;
        case CREATE_FILE:
            break;
        case FILE_BLOCK:
            break;
        default:
            break;
    }

}

/* read the msg */
int readMsg(int sockfd, void *buff, enum MessageType mt) {
    switch (mt) {
        case SUCCESS:
            break;
        case FAILURE:
            break;
        case CREATE_FILE:
            break;
        case FILE_BLOCK:
            break;
        default:
            break;
    }
}


/**
int write_msg(int sockfd, char *buff, size_t len) {
    Msg message;
    memset(&message, 0, sizeof(message));
    strcpy(message.head, "iotek2012");
    memcpy(message.buff, buff, len);
    message.checknum = msg_check(&message);
    if (write(sockfd, &message, sizeof(message)) != sizeof(message)) {
        return -1;
    }
}

int read_msg(int sockfd, char *buff, size_t len) {
    Msg message;
    memset(&message, 0, sizeof(message));
    size_t size;
    if ((size = read(sockfd, &message, sizeof(message))) < 0) {
        return -1;
    } else if (size == 0){
        return 0;
    }
    unsigned char s = msg_check(&message);
    if ((s == (unsigned char)message.checknum)
            && (!strcmp("iotek2012", message.head))) {
        memcpy(buff, message.buff, len);
        return sizeof(message);
    }
    return -1;
}

**/