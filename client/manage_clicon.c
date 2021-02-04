#include "manage_clicon.h"

int manage_connection(int sfd, struct sockaddr *addr){
    // char *msg = "Message";
    // size_t msgLen = strlen(msg);

    printf("Connection successful\n");

    char input[1024];
    // char *input = 
    while (1){
        memset(input, 0, 1024);
        int i = 0;
        while ((input[i++] = getchar()) != '\n');
        input[i-1]='\0';
        // *input = 0;
        // printf("a:\n");
        // scanf("%1023[^\n]", input);
        size_t inputLen = strlen(input);

        if ( write(sfd, input, inputLen) != inputLen ){
            perror("Failed send message to socket");
            exit(EXIT_FAILURE);
        }
    }
    

    return 0;
}