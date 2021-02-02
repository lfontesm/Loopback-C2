#if !defined(CLIENT_CON)
#define CLIENT_CON

#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int manage_connection(int sfd, struct sockaddr *addr);


#endif // CLIENT_CON
