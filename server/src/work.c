#include "work.h"

int NewSocket() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		err_quit("NewSocket failed!");
	}
	return sockfd;
}

int ServerInit(struct sockaddr_in *sa, in_port_t port) {
	int listenfd = NewSocket();
	memset(sa, 0, sizeof(*sa));
	sa->sin_family = AF_INET;
	sa->sin_port = port;
	sa->sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listenfd, (struct sockaddr *)sa, sizeof(*sa)) < 0) {
		err_quit("Server bind failed!");
	}

	if (listen(listenfd, LISTEN_QUEUE_SIZE) < 0) {
		err_quit("Server listen failed!");
	}

	return listenfd;
}