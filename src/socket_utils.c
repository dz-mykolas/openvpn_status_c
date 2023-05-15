#include "socket_utils.h"

int socket_connect(struct sockaddr_in *server, int *socket_desc)
{
    *socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (*socket_desc == -1) {
        printf("Could not create socket\n");
        return EXIT_FAILURE;
    }
    server->sin_addr.s_addr = inet_addr("192.168.1.163");
    server->sin_family = AF_INET;
    server->sin_port = htons(7505);

    if (connect(*socket_desc, (struct sockaddr *)server, sizeof(*server)) != 0) {
        printf("connect error\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int socket_send_message(int socket_desc, char *message, char *server_reply)
{
    if (send(socket_desc, message, strlen(message), 0) < 0) {
        printf("Send failed");
        return 1;
    }
    
    return socket_receive_message(socket_desc, server_reply);
}

int socket_receive_message(int socket_desc, char *server_reply)
{
    int recv_size = 0;
    if ((recv_size = recv(socket_desc, server_reply, MAX_BUFFER_SIZE, 0)) < 0) {
        printf("recv failed");
        return 1;
    }

    return 0;
}
