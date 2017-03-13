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
    int         fileSize;                       // file size
    int         blockCount;                     // number of block
    int         blockSize;                      // the size of every block
}filehead_t;

typedef struct fileblock {
    char        fileName[FILENAME_MAXLEN];      // file name
    size_t      offset;                         // offset
    size_t      len;                            // block length
    char        content[BUFFSIZE];              // the content
}fileblock_t;

typedef struct message{
    enum        MessageType     type;
    unsigned    short           checknum;
    char                        buf[sizeof(fileblock_t) + 10];
}msg_t;

/* write the msg */
//int writeMsg(int sockfd, void *buff, enum MessageType mt);

/* read the msg */
//int readMsg(int sockfd, void *buff, enum MessageType mt);

#endif /** __INCLUDE_PIGEON_PROTOCOL_H__ **/