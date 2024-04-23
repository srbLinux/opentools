#include "debug.h"

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#if __linux__
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include "../defines.h"

typedef struct {
    int port;
    char *url;
    int sockfd;
} TcpSocket;

static TcpSocket *connectToServer(const char *url, int port, const char *msg);
static void recvServerMessage(TcpSocket *socket);

char *OTS_Debug_FormatTimeNow() {
    char *strtime = (char *)malloc(sizeof(char)*32);
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(strtime, 32, "[%Y-%m-%d %H:%M:%S]", timeinfo);
    return strtime;
}

#if __linux__
int uploadMessageToServer(const char *url, int port, const char *msg) {
    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd==-1) {

    }
}
#endif


