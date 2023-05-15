#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "defs.h"

int socket_connect(struct sockaddr_in *server, int *socket_desc);
int socket_send_message(int socket_desc, char *message, char *server_reply);
int socket_receive_message(int socket_desc, char *server_reply);

#endif