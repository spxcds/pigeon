#ifndef __INCLUDE_PIGEON_PROTOCOL_H__
#define __INCLUDE_PIGEON_PROTOCOL_H__

#define BUFFSIZE           1024     /** file buffsize **/
#define FILENAME_MAXLEN    128      /** file name max length **/

enum MessageType {
    SUCCESS,
    FAILURE,
    CREATE_FILE,
    FILE_BLOCK
};

enum ErrorType {
    BrokenPipe
};

typedef struct filehead {
    char        fileName[FILENAME_MAXLEN];      // file name
    size_t         fileSize;                       // file size
//    int         blockCount;                     // number of block
//    int         blockSize;                      // the size of every block
}filehead_t;

/**
typedef struct content
{
    size_t      len;                            // the buf content
    char        buf[0];                         // the first address of the buf
}content_t;
**/

typedef struct fileblock {
    char        fileName[FILENAME_MAXLEN];      // file name
    size_t      offset;                         // offset
    size_t      len;                            // block length
    char        buf[0];                            // the content;
}fileblock_t;

typedef struct message{
    enum        MessageType     type;
    unsigned    short           checkNum;
    size_t                      bufLen;
    char                        buf[0];
}msg_t;

int readFileHeadFromBuf(filehead_t *fileHead, void *buf_, int len);
int writeFileHeadToBuf(filehead_t *fileHead, void *buf_, int len);
int writeMsg(int sockfd, enum MessageType mt, 
                        void *buf, int bufLen);

int readMsg(int sockfd, enum MessageType *mt,
                        void *buf, int *bufLen);
/* write the msg */
//int writeMsg(int sockfd, void *buff, enum MessageType mt);

/* read the msg */
//int readMsg(int sockfd, void *buff, enum MessageType mt);

#endif /** __INCLUDE_PIGEON_PROTOCOL_H__ **/