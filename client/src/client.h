#ifndef __CLIENT_CLIENT_H__
#define __CLIENT_CLIENT_H__

#define     THREADNUM       1

typedef struct  fdset {
    int         sockfdArray[THREADNUM];
    int         filefdArray[THREADNUM];
    sem_t       isFree[THREADNUM];
}fdset_t;

int BuildConnection(const char *ip, int *fdArray, int len);

int NewSocket();

struct sockaddr_in *NewServerAddr(int sockfd, const char *ip);    

int FdsetInit(fdset_t *fdSet);

int FdsetDestroy(fdset_t *fdSet);

#endif /** __CLIENT_CLIENT_H__ **/