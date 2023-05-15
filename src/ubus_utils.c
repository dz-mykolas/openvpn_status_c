#include "ubus_utils.h"

static const struct ubus_method openvpn_clients_methods[] = {
    UBUS_METHOD_NOARG("get", current_clients_get),
};

static struct ubus_object_type openvpn_clients_object_type =
    UBUS_OBJECT_TYPE("openvpn_clients", openvpn_clients_methods);

static struct ubus_object openvpn_clients_object = {
    .name = "openvpn_clients",
    .type = &openvpn_clients_object_type,
    .methods = openvpn_clients_methods,
    .n_methods = ARRAY_SIZE(openvpn_clients_methods),
};

int start_ubus(struct ubus_context **ctx)
{
    uloop_init();
    *ctx = ubus_connect(NULL);
    if (*ctx == NULL) {
        log_event(LOG_ERR, "Failed to connect to ubus");
        return EXIT_FAILURE;
    }
    ubus_add_uloop(*ctx);
    if (ubus_add_object(*ctx, &openvpn_clients_object) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    uloop_run();
	uloop_done();
    return EXIT_SUCCESS;
}

static int current_clients_get(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_buf b = {};
	blob_buf_init(&b, 0);
    struct Client *list = NULL;
    if (get_connected_clients(&list) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    void *array = blobmsg_open_array(&b, "connected_clients");
    struct Client *temp = list;
    while (temp != NULL) {
        void *table = blobmsg_open_table(&b, "");
        blobmsg_add_string(&b, "common_name", temp->common_name);
        blobmsg_add_string(&b, "real_address", temp->real_address);
        char buffer[64];
        sprintf(buffer, "%lu", temp->bytes_received);
        blobmsg_add_string(&b, "bytes_received", buffer);
        sprintf(buffer, "%lu", temp->bytes_sent);
        blobmsg_add_string(&b, "bytes_sent", buffer);
        blobmsg_add_string(&b, "connected_since", temp->connected_since);

        blobmsg_close_table(&b, table);
        temp = temp->next;
    }
    blobmsg_close_array(&b, array);
	int ret = ubus_send_reply(ctx, req, b.head);
    clients_remove(&list);
	blob_buf_free(&b);
	return ret;
}
