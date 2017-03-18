#ifndef __CLIENT_WORK_H__
#define __CLIENT_WORK_H__

#include "client.h"

//int SendFile(const char *fileName, int sockfd);

typedef struct sendFileBlockArg{
    fdset_t         *fdSet;
    int             idx;
    size_t          offset;
    size_t          len;
}SendFileBlockArg_t;

int SendFile(const char *fileName, fdset_t *fdSet);

#endif /** __CLIENT_WORK_H__ **/