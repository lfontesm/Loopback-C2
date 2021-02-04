#include "manage_secon.h"

int manage_connection(int sfd, struct sockaddr *addr, socklen_t addrlen){
    char buf[128];
    int BUF_SIZE = 128;

    puts("Client connected");
    while (1){
        int afd = accept(sfd, addr, &addrlen);
        if (afd == -1){
            perror("Failed to accept\n");
            exit(EXIT_FAILURE);
        }
        // struct sockaddr connectionStruct;
        // socklen_t a = sizeof(connectionStruct);
        memset(buf, 0, 128);
        // *buf = 0;
        // ssize_t nread = recvfrom(sfd, buf, BUF_SIZE, 0, addr, &addrlen);
        ssize_t nread = read(afd, buf, BUF_SIZE);
        if (nread == -1){
            perror("Failed to recv message\n");
            exit(EXIT_FAILURE);
        }

        // collect_input(afd, buf, BUF_SIZE);

        // int res = sanitize_input(buf, afd);
        // if (res == 2) break;
        // TODO: stablish a more refined communication protocol
        printf("Recieved msg: .%s.\n", buf);
        // fsync(afd);
        close(afd);
        // sanitize_input
    }
    return 1;
}

int collect_input(int afd, char *buf, size_t BUF_SIZE){
    int i = 0;
    ssize_t nread;
    while ((nread = read(afd, &buf[i++], 1)) != -1 && i < BUF_SIZE ) {
        if (buf[i-1] == '\n') {
            buf[i-1] = '\0';
            break;
        }
    }
    printf("%d\n",i);
    if (nread == -1) {
        perror("Failed to read\n");
        exit(EXIT_FAILURE);
    }

    return 1;
}

int sanitize_input(char *userInput, int afd){
    if (!userInput) return 0;
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