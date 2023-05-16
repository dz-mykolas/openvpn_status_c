#include "task_utils.h"

void log_event(int type, const char *format, ...)
{
	setlogmask(LOG_UPTO(LOG_NOTICE));
	openlog("openvpn_status", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);

	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	syslog(type, "%s", buffer);
	closelog();
}

// TODO:
// errno does not work for some reason
struct Client *client_parse(char *line)
{
    char common_name[MAX_BUFFER_SIZE];
    char real_address[MAX_BUFFER_SIZE];
    u_int64_t bytes_received;
    u_int64_t bytes_sent;
    char connected_since[MAX_BUFFER_SIZE];

    snprintf(common_name, MAX_BUFFER_SIZE, "%s", strtok(line, ","));
    snprintf(real_address, MAX_BUFFER_SIZE, "%s", strtok(NULL, ","));
    bytes_received = strtoul(strtok(NULL, ","), NULL, 0);
    if (errno == ERANGE)
        perror("Invalid value\n");
    bytes_sent = strtoul(strtok(NULL, ","), NULL, 0);
    if (errno == EINVAL)
        perror("Invalid value\n");
    snprintf(connected_since, MAX_BUFFER_SIZE, "%s", strtok(NULL, ","));

    return client_create(common_name, real_address, bytes_received, bytes_sent, connected_since);
}

/* Loops between: "OpenVPN CLIENT LIST" and "ROUTING TABLE" */
int status_parse_loop_clients(struct Client **list, char *line, char *saveptr_data)
{
    while ((line = strtok_r(NULL, "\n", &saveptr_data)) && strstr(line, "ROUTING TABLE") == NULL) {
        if (strstr(line, "Updated,") != NULL || strstr(line, "Common Name,Real Address,Bytes Received,Bytes Sent,Connected Since") != NULL)
            continue;
        struct Client *new = client_parse(line);
        if (new)
            clients_add_front(list, new);
    }

    return 0;
}

int status_parse(struct Client **list, char *message)
{
    char *saveptr_data;
    char *line = strtok_r(message, "\n", &saveptr_data);
    while (line != NULL) {
        if (strstr(line, "OpenVPN CLIENT LIST") == NULL) {
            return EXIT_FAILURE;
        } else {
            status_parse_loop_clients(list, line, saveptr_data);
        }
        line = strtok_r(NULL, "\n", &saveptr_data);
    }
}

int socket_message_helper(char *message, char server_reply[MAX_BUFFER_SIZE * MAX_CLIENTS])
{
    int socket_desc;
    struct sockaddr_in server;
    if (socket_connect(&server, &socket_desc) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    if (socket_receive_message(socket_desc, server_reply) != EXIT_SUCCESS) {
        close(socket_desc);
        return EXIT_FAILURE;
    }
    if (socket_send_message(socket_desc, message, server_reply) != EXIT_SUCCESS) {
        close(socket_desc);
        return EXIT_FAILURE;
    }
    close(socket_desc);

    return EXIT_SUCCESS;
}

int kill_connected_client(char *command, char server_reply[MAX_BUFFER_SIZE * MAX_CLIENTS])
{
    int socket_desc;
    struct sockaddr_in server;

    if (socket_message_helper(command, server_reply) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int get_connected_clients(struct Client **list)
{
    char server_reply[MAX_BUFFER_SIZE * MAX_CLIENTS];

    if (socket_message_helper("status\n", server_reply) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    status_parse(list, server_reply);

    return EXIT_SUCCESS;
}