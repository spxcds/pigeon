#ifndef __CLIENT_WORK_H__
#define __CLIENT_WORK_H__

#include "client.h"

//int SendFile(const char *fileName, int sockfd);

typedef struct sendFileBlockArg{
    int             sockfd;
    int             filefd;
    sem_t           hasResource;
    size_t          offset;
    size_t          len;
}SendFileBlockArg_t;

int SendFile(const char *fileName, fdset_t *fdSet);

#endif /** __CLIENT_WORK_H__ **/