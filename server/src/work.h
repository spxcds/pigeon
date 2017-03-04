#ifndef __SERVER_WORK_H__
#define __SERVER_WORK_H__

#define	LISTEN_QUEUE_SIZE	20			/** listen queue size **/
#define EPOLL_SIZE			100			/** epoll listened size **/

#include "pigeon.h"
#include "utils.h"

/** new a socket **/
int NewSocket();

/** initial server socket, with passed struct sockaddr and port **/
int ServerInit(struct sockaddr_in *sa, in_port_t port);

/** receive the file **/
int RecvFile(const char *fileName, int sockfd);

#endif /** __SERVER_WORK_H__ **/