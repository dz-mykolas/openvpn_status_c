#include "main.h"

int main()
{
    int socket_desc;
    struct sockaddr_in server;
    char server_reply[MAX_BUFFER_SIZE * MAX_CLIENTS];

    if (socket_connect(&server, &socket_desc) == 1)
        return EXIT_FAILURE;
    // Initial "connect" message
    if (socket_receive_message(socket_desc, server_reply) != 0)
        return EXIT_FAILURE;
    printf("%s", server_reply);
    socket_send_message(socket_desc, "status\n", server_reply);
    //printf("%s", server_reply);

    struct Client *list = NULL;
    status_parse(&list, server_reply);
    clients_print(list);
    clients_remove(&list);
    
    return EXIT_SUCCESS;
}
