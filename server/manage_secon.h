#if !defined(SERVER_CON)
#define SERVER_CON

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

int manage_connection(int sfd, struct sockaddr *addr, socklen_t addrlen);
int sanitize_input(char *userInput, int afd);
int collect_input(int afd, char *buf, size_t BUF_SIZE);

#endif // SERVER_CON
