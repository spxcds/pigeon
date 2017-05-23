#include "pigeon.h"
#include "utils.h"
#include "client.h"
#include "work.h"
#include "string.h"

int processInput(char *input, char *ip, char *fileName) {
    int len = strlen(input);
    int mid;
    for (int i = 0; input[i] && input[i] != ':'; ++i) {
        ip[i] = input[i];
        ip[i + 1] = 0;
        mid = i;
    }

    if (mid >= len) {
        return -1;
    }
    mid += 2;

    for (int i = 0; input[i + mid]; ++i) {
        fileName[i] = input[i + mid];
        fileName[i + 1] = 0;
    }

    return 0;
}

int main(int argc, char **argv) {

    if (argc != 3) {
        perror("usage: pigeon <IPaddress>:<remote file> <local file>");
        exit(-1);
    }

    fdset_t fdSet;
    FdsetInit(&fdSet);

    char ip[200];
    char fileName[200];
    if (processInput(argv[1], ip, fileName) != 0) {
        perror("Invalid input");
        exit(-1);
    }

    BuildConnection(ip, fdSet.sockfdArray, THREADNUM);

    SendFile(argv[2], fileName, &fdSet);
    
    FdsetDestroy(&fdSet);
    return 0;
}