#ifndef UBUS_UTILS_H
#define UBUS_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include <syslog.h>
#include <string.h>

#include "client.h"
#include "task_utils.h"

enum {
    CLIENT_COMMON_NAME,
    CLIENT_REAL_ADDRESS,
    CLIENT_BYTES_RECEIVED,
    CLIENT_BYTES_SENT,
    CLIENT_CONNECTED_SINCE,
    __CLIENT_MAX,
};

int start_ubus(struct ubus_context **ctx);

static int current_clients_get(struct ubus_context *ctx, struct ubus_object *obj,
            struct ubus_request_data *req, const char *method,
            struct blob_attr *msg);
            
#endif