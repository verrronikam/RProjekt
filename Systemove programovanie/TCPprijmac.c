#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int call_simplerecv_arg(int argc, char **argv) {
    if (argc != 2) {
        printf("Invalid arguments!\n");
        return -1;
    }

    char *endptr;
    long port_long = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || port_long < 0 || port_long > 65535) {
        fprintf(stderr, "Invalid port\n");
        return -1;
    }
    int port = (int) port_long;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return -1;
    }

    char buffer[4096];
    ssize_t bytes_received;
    char *message = NULL;
    size_t total_received = 0;

    while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        char *temp = realloc(message, total_received + bytes_received);
        if (!temp) {
            perror("realloc");
            free(message);
            close(client_fd);
            close(server_fd);
            return -1;
        }
        message = temp;
        memcpy(message + total_received, buffer, bytes_received);
        total_received += bytes_received;
    }

    if (bytes_received < 0) {
        perror("recv");
        free(message);
        close(client_fd);
        close(server_fd);
        return -1;
    }

    if (message) {
        fwrite(message, 1, total_received, stdout);
        free(message);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}