#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "rc4.h"

int main(int argc, char **argv){
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    char *key = "LECNAAEAAE";

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

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

    int yes=1;
    //char yes='1'; // Solaris people use this

    // lose the pesky "Address already in use" error message
    if (setsockopt(soc, SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    } 


    err = bind(soc, servinfo->ai_addr, servinfo->ai_addrlen);
    if (err == -1) {
        perror("Failed to bind a socket");
        exit(EXIT_FAILURE);
    }

    err = listen(soc, 5);
    if (err == -1) {
        perror("Failed to listen on socket");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    char BUF[1024];
    int i;
    int afd;
    struct sockaddr_storage their_addr;
    socklen_t addrSize = sizeof their_addr;
    while ((afd = accept(soc, (struct sockaddr *)&their_addr, &addrSize)) != -1){
        while (1){
            i = recv(afd, BUF, 1024, 0);
            rc4((unsigned char *)key, strlen(key), BUF, strlen(BUF));
            if (i == -1 || i == 0)
                continue;
            puts(BUF); 
            // printf("%d\n",i);
            if (strcmp("quit", BUF) == 0){
                send(afd, "aa", 2, 0);
                close(afd);
                break;
            }
            send(afd, "a", 1, 0);
            memset(BUF, 0, 1024);
        }
    }
    
    return 0;
}