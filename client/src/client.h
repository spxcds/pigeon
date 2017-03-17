#ifndef __CLIENT_CLIENT_H__
#define __CLIENT_CLIENT_H__

int NewSocket();

struct sockaddr_in *NewServerAddr(int sockfd, const char *ip);    

#endif /** __CLIENT_CLIENT_H__ **/