#ifndef CLIENTS_H
#define CLIENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "defs.h"

struct Client {
    char common_name[MAX_BUFFER_SIZE];
    char real_address[MAX_BUFFER_SIZE];
    u_int64_t bytes_received;
    u_int64_t bytes_sent;
    char connected_since[MAX_BUFFER_SIZE];
    
    struct Client *next;
};

struct Client *client_create(char *common_name, char *real_address, u_int64_t bytes_received, u_int64_t bytes_sent, char *connected_since);
void client_print(struct Client *client);

void clients_add_front(struct Client **list, struct Client *new);
void clients_print(struct Client *list);
void clients_remove(struct Client **list);

#endif