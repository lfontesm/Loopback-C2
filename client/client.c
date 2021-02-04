#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "manage_clicon.h"

int main(int agrc, char **argv){
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(NULL, "8080", &hints, &servinfo)) != 0 ) {
        fprintf(stderr, "getaddrinfo error %s\n", gai_strerror(status));
        exit(1);
    }

    int err, soc;
    soc = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (soc == -1) {
        perror("Failed to allocate a socket");
        exit(EXIT_FAILURE);
    }

    err = connect(soc, servinfo->ai_addr, servinfo->ai_addrlen);
    if (err == -1){
        perror("Failed to connect");
        exit(EXIT_FAILURE);
    }

    char BUF[1024];
    // size_t BUF_LEN;
    while (1){
        scanf("%1023[^\n]", BUF);
        // BUF_LEN = strlen(BUF);
        // BUF[BUF_LEN]
        send(soc, BUF, strlen(BUF), 0);
        getchar();
        memset(BUF, 0, 1024);
    }
    
    
    return 0;
}