#if !defined(SERVER_CON)
#define SERVER_CON

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

int manage_connection(int sfd, struct sockaddr *addr);

#endif // SERVER_CON
