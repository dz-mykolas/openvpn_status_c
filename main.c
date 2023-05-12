#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

struct Client {
    char common_name[MAX_BUFFER_SIZE];
    char real_address[MAX_BUFFER_SIZE];
    u_int64_t bytes_received;
    u_int64_t bytes_sent;
    char connected_since[MAX_BUFFER_SIZE];
    
    struct Client *next;
};

int status_parse();
struct Client *clients_parse(char *line);
struct Client *client_create(char *common_name, char *real_address, u_int64_t bytes_received, u_int64_t bytes_sent, char *connected_since);

int main()
{
    // int socket_desc;
    // struct sockaddr_in server;
    // char message[MAX_BUFFER_SIZE];
    // char server_reply[MAX_BUFFER_SIZE];

    // socket_connect(&server, &socket_desc);
    // socket_send_message(socket_desc, message, server_reply);
    status_parse();
    return 0;
}

// int socket_connect(struct sockaddr_in *server, int *socket_desc)
// {
//     socket_desc = socket(AF_INET, SOCK_STREAM, 0);
//     if (socket_desc == -1) {
//         printf("Could not create socket");
//         return 1;
//     }
//     server->sin_addr.s_addr = inet_addr("localhost");
//     server->sin_family = AF_INET;
//     server->sin_port = htons(7505);

//     if (connect(socket_desc, (struct sockaddr *)server, sizeof(*server)) != 0) {
//         printf("connect error");
//         return 1;
//     }

//     return 0;
// }

// int socket_send_message(int socket_desc, char *message, char *server_reply)
// {
//     if (send(socket_desc, message, strlen(message), 0) < 0) {
//         printf("Send failed");
//         return 1;
//     }

//     if (recv(socket_desc, server_reply, MAX_BUFFER_SIZE, 0) < 0) {
//         printf("recv failed");
//         return 1;
//     }

//     return 0;
// }

int status_parse(/* char *message */)
{
        char data[] = "OpenVPN CLIENT LIST\n"
                  "Updated,2023-05-12 14:53:27\n"
                  "Common Name,Real Address,Bytes Received,Bytes Sent,Connected Since\n"
                  "r2,192.168.1.1:46017,3461,3113,2023-05-12 14:53:11\n"
                  "r1,192.168.1.1:37973,16420,39656,2023-05-12 13:58:42\n"
                  "ROUTING TABLE\n"
                  "Virtual Address,Common Name,Real Address,Last Ref\n"
                  "10.8.0.6,r1,192.168.1.1:37973,2023-05-12 13:58:42\n"
                  "10.8.0.10,r2,192.168.1.1:46017,2023-05-12 14:53:11\n"
                  "GLOBAL STATS\n"
                  "Max bcast/mcast queue length,1\n"
                  "END\n";

    char *line = strtok(data, "\n");
    while (line != NULL) {
        if (strstr(line, "OpenVPN CLIENT LIST") != NULL)
            clients_parse(line);
        line = strtok(NULL, "\n");
    }
}

struct Client *clients_parse(char *line)
{
    struct Client *client_list = NULL;

    char common_name[MAX_BUFFER_SIZE];
    char real_address[MAX_BUFFER_SIZE];
    u_int64_t bytes_received;
    u_int64_t bytes_sent;
    char connected_since[MAX_BUFFER_SIZE];

    strtok(NULL, "\n");
    if (strstr(line, "Updated,") != NULL)
        return NULL;
    strtok(NULL, "\n");
    if (strstr(line, "Common Name,Real Address,Bytes Received,Bytes Sent,Connected Since") != NULL)
        return NULL;

    // TODO:
    // Make sscanf safe or user other
    while ((line = strtok(NULL, "\n")) && strstr(line, "ROUTING TABLE") == NULL) {
        sscanf(line, "%[^,],%[^,],%lu,%lu,%[^\n]", common_name, real_address,   
                &bytes_received, &bytes_sent, connected_since);
        printf("Client: %s, Real Address: %s, Bytes Received: %lu, Bytes Sent: %lu, Connected Since: %s\n",
                common_name, real_address, bytes_received, 
                bytes_sent, connected_since);
        struct Client *new = client_create(common_name, real_address, bytes_received, bytes_sent, connected_since);
    }

    return client_list;
}

struct Client *client_create(char *common_name, char *real_address, u_int64_t bytes_received, u_int64_t bytes_sent, char *connected_since)
{
    struct Client *new = NULL;

    return new;
}