#include "pigeon.h"
#include "utils.h"
#include "client.h"
#include "work.h"

int main(int argc, char **argv) {

    if (argc != 2) {
        perror("usage: pigeon <local file> <IPaddress>");
        exit(-1);
    }

    fdset_t fdSet;
    FdsetInit(&fdSet);

    const char *ip = "127.0.0.1";
    
    BuildConnection(ip, fdSet.sockfdArray, THREADNUM);

    SendFile(argv[1], &fdSet);
    
    FdsetDestroy(&fdSet);
    printf("send success\n");
    return 0;
}