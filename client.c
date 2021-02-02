#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

int main(int agrc, char **argv){
    const char *port = "8080";
    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    int getAddrInfoRet;
    if ( getAddrInfoRet = getaddrinfo(NULL, port, &hints, &res) != 0 ){
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
        int conRet = connect(sfd, rp->ai_add, rp->ai_addrlen);
        if (conRet == -1){
            perror("Failed to connect");
            exit(EXIT_FAILURE);
        }

        else printf("Connection succesfull!\n");
    }

    return 0;
}