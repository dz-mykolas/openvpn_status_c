#ifndef TASK_UTILS_H
#define TASK_UTILS_H

#include <sys/types.h>
#include <errno.h>

#include "defs.h"
#include "clients.h"

int status_parse(struct Client **list, char *message);
struct Client *client_parse(char *line);

#endif