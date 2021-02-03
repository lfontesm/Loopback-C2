#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "manage_secon.h"

int main(int argc, char **argv){
    const char *port = "8080";
    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    int getAddrInfoRet;
    if ( (getAddrInfoRet = getaddrinfo(NULL, port, &hints, &res)) != 0 ){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getAddrInfoRet));
        exit(EXIT_FAILURE);
    }

    struct addrinfo *rp;
    int sfd, bindRet;
    for (rp = res; rp != NULL; rp = rp->ai_next){
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        bindRet = bind(sfd, rp->ai_addr, rp->ai_addrlen);
        if (bindRet == 0)
            break;

        close(sfd);
    }

    if (rp == NULL) {
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    if (bindRet == 0){
        int listenRet = listen(sfd, 2);
        if (listenRet == -1){
            perror("Failed to listen\n");
            exit(EXIT_FAILURE);
        }

        int afd = accept(sfd, rp->ai_addr, &rp->ai_addrlen);
        if (afd == -1){
            perror("Failed to accept\n");
            exit(EXIT_FAILURE);
        }

        struct sockaddr connectionStruct;
        manage_connection(afd, &connectionStruct);
    }

    freeaddrinfo(res);

    return 0;
}