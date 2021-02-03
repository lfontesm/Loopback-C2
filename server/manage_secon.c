#include "manage_secon.h"

int manage_connection(int sfd, struct sockaddr *addr, socklen_t addrlen){
    char buf[128];
    int BUF_SIZE = 128;
    memset(buf, 0, 128);

    int afd = accept(sfd, addr, &addrlen);
    if (afd == -1){
        perror("Failed to accept\n");
        exit(EXIT_FAILURE);
    }
    puts("Client connected");
    while (1){
        // struct sockaddr connectionStruct;
        // socklen_t a = sizeof(connectionStruct);
        ssize_t nread = recvfrom(afd, buf, BUF_SIZE, 0, addr, &addrlen);
        if (nread == -1){
            perror("Failed to recv message\n");
            exit(EXIT_FAILURE);
        }


        int res = sanitize_input(buf, afd);
        if (res == 2) break;
        // TODO: stablish a more refined communication protocol
        printf("Recieved msg: %s\n", buf);

        // sanitize_input
    }
    return 1;
}

int sanitize_input(char *userInput, int afd){
    char *cmd = strtok(userInput, " ");
    if (strcmp(cmd, "get") == 0){
        // exfiltrate_data()
    }

    if (strcmp(cmd, "exit") == 0){
       close(afd);
       return 2;
    }
    
    return -1;
}