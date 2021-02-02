#include "manage_secon.h"

int manage_connection(int sfd, struct sockaddr *addr){
    char buf[128];
    int BUF_SIZE = 128;
    socklen_t addrLen = sizeof(addr);
    ssize_t nread = recvfrom(sfd, buf, BUF_SIZE, 0, addr, &addrLen);

    if (nread == -1){
        perror("Failed to recv message");
        exit(EXIT_FAILURE);
    }

    printf("Recieved msg: %s\n", buf);

    return 1;
}