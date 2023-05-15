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
    // char data[] = "OpenVPN CLIENT LIST\n"
    //             "Updated,2023-05-12 14:53:27\n"
    //             "Common Name,Real Address,Bytes Received,Bytes Sent,Connected Since\n"
    //             "r2,192.168.1.1:46017,3461,3113,2023-05-12 14:53:11\n"
    //             "r1,192.168.1.1:37973,16420,39656,2023-05-12 13:58:42\n"
    //             "ROUTING TABLE\n"
    //             "Virtual Address,Common Name,Real Address,Last Ref\n"
    //             "10.8.0.6,r1,192.168.1.1:37973,2023-05-12 13:58:42\n"
    //             "10.8.0.10,r2,192.168.1.1:46017,2023-05-12 14:53:11\n"
    //             "GLOBAL STATS\n"
    //             "Max bcast/mcast queue length,1\n"
    //             "END\n";

    char *saveptr_data;
    char *line = strtok_r(message, "\n", &saveptr_data);
    while (line != NULL) {
        if (strstr(line, "OpenVPN CLIENT LIST") == NULL) {
            return 1;
        } else {
            status_parse_loop_clients(list, line, saveptr_data);
        }
        line = strtok_r(NULL, "\n", &saveptr_data);
    }
}

int get_connected_clients(struct Client **list)
{
    int socket_desc;
    struct sockaddr_in server;
    char server_reply[MAX_BUFFER_SIZE * MAX_CLIENTS];

    if (socket_connect(&server, &socket_desc) == 1)
        return EXIT_FAILURE;
    // Initial "connect" message
    if (socket_receive_message(socket_desc, server_reply) != 0)
        return EXIT_FAILURE;
    socket_send_message(socket_desc, "status\n", server_reply);
    close(socket_desc);
    status_parse(list, server_reply);

    if (*list == NULL)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}