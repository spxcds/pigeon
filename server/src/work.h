#ifndef __SERVER_WORK_H__
#define __SERVER_WORK_H__

typedef struct RecvFileArg {
    size_t          sockfd;
}RecvFileArg_t;

/** receive the file **/
void *RecvFile(void *arg);


#endif /** __SERVER_WORK_H__ **/