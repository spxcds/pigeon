#include "pigeon.h"
#include "daemonize.h"

#include "work.h"

// server
int main() {
//    daemonize("pigeon");

    struct sockaddr_in servaddr;
    int listenfd = ServerInit(&servaddr, SERV_PORT);

    for (;;) {
    	printf("I'm in for loop\n");
    	sleep(1);
    }

    return 0;
}
