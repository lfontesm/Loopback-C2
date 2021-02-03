#include "manage_clicon.h"

int manage_connection(int sfd, struct sockaddr *addr){
    // char *msg = "Message";
    // size_t msgLen = strlen(msg);

    printf("Connection successful\n");

    while (1){
        char input[1024];
        scanf("%1023[^\n]", input);
        size_t inputLen = strlen(input);

        if ( write(sfd, input, inputLen) != inputLen ){
            perror("Failed send message to socket");
            exit(EXIT_FAILURE);
        }
    }
    

    return 0;
}