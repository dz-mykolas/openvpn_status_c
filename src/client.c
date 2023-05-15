#include "client.h"

struct Client *client_create(char *common_name, char *real_address, u_int64_t bytes_received, u_int64_t bytes_sent, char *connected_since)
{
    struct Client *new = NULL;

    new = (struct Client *)malloc(sizeof(struct Client));
    snprintf(new->common_name, MAX_BUFFER_SIZE, "%s", common_name);
    snprintf(new->real_address, MAX_BUFFER_SIZE, "%s", real_address);
    new->bytes_received = bytes_received;
    new->bytes_sent = bytes_sent;
    snprintf(new->connected_since, MAX_BUFFER_SIZE, "%s", connected_since);

    return new;
}

void clients_add_front(struct Client **list, struct Client *new)
{
    new->next = *list;
    *list = new;
}

void client_print(struct Client *client)
{
    printf("Client: %s, Real Address: %s, Bytes Received: %lu, Bytes Sent: %lu, Connected Since: %s\n",
            client->common_name, client->real_address, client->bytes_received, 
            client->bytes_sent, client->connected_since);
}

void clients_print(struct Client *list)
{
    struct Client *temp = list;
    while (temp != NULL) {
        client_print(temp);
        temp = temp->next;
    }
}

void clients_remove(struct Client **list)
{
    struct Client *temp = *list;
    struct Client *to_free = NULL;
    while (temp != NULL) {
        to_free = temp;
        temp = temp->next;
        free(to_free);
    }
    *list = NULL;
}