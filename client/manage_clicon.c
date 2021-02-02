#include "manage_clicon.h"

int manage_connection(int sfd, struct sockaddr *addr){
    char *msg = "Message";
    size_t msgLen = strlen(msg);

    printf("Connection successful\n");

    if ( write(sfd, msg, msgLen) != msgLen ){
        perror("Failed send message to socket");
        exit(EXIT_FAILURE);
    }
}