#ifndef TASK_UTILS_H
#define TASK_UTILS_H

#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include "defs.h"
#include "client.h"
#include "socket_utils.h"

void log_event(int type, const char *format, ...);
int status_parse(struct Client **list, char *message);
struct Client *client_parse(char *line);
int socket_message_helper(char *message, char server_reply[MAX_BUFFER_SIZE * MAX_CLIENTS]);
int kill_connected_client(char *command, char server_reply[MAX_BUFFER_SIZE * MAX_CLIENTS]);
int get_connected_clients(struct Client **list);

#endif