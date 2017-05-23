#include "pigeon.h"
#include "utils.h"
#include "client.h"
#include "work.h"
#include "string.h"

int is_valid_ip(const char *ip) 
{ 
    int section = 0;  //每一节的十进制值 
    int dot = 0;       //几个点分隔符 
    int last = -1;     //每一节中上一个字符 
    while(*ip){ 
        if(*ip == '.'){ 
            dot++; 
            if(dot > 3){ 
                return 0; 
            } 
            if(section >= 0 && section <=255){ 
                section = 0; 
            }else{ 
                return 0; 
            } 
        }else if(*ip >= '0' && *ip <= '9'){ 
            section = section * 10 + *ip - '0'; 
            if(last == '0'){ 
                return 0; 
            } 
        }else{ 
            return 0; 
        } 
        last = *ip; 
        ip++;        
    }

    if(section >= 0 && section <=255){ 
        section = 0; 
    } 
    return 1; 
}

int processInput(char *input, char *ip, char *fileName) {
    int len = strlen(input);
    int mid;
    for (int i = 0; input[i] && input[i] != ':'; ++i) {
        ip[i] = input[i];
        ip[i + 1] = 0;
        mid = i;
    }

    if (is_valid_ip(ip) == 0) {
        return -1;
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
        printf("usage: pigeon <IPaddress>:<remote file> <local file>\n");
        exit(-1);
    }

    fdset_t fdSet;
    FdsetInit(&fdSet);

    char ip[200];
    char fileName[200];
    if (processInput(argv[1], ip, fileName) != 0) {
        printf("Invalid input");
        exit(-1);
    }

    BuildConnection(ip, fdSet.sockfdArray, THREADNUM);

    SendFile(argv[2], fileName, &fdSet);
    
    FdsetDestroy(&fdSet);
    return 0;
}