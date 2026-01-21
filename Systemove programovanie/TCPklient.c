#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/select.h>

int call_send_hello_arg(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stdout, "Invalid arguments!\n");
        return 1;
    }

    const char *ip_address = argv[1];
    const char *port_str = argv[2];
    char *endptr;
    long port_long = strtol(port_str, &endptr, 10);
    if (*endptr != '\0' || port_long < 1 || port_long > 65535) {
        fprintf(stdout, "Invalid arguments!\n");
        return 1;
    }
    int port = (int) port_long;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    //Nastavenie socketu na neblokujuci rezim
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl");
        close(sockfd);
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return 1;
    }

    int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (err < 0) {
        if (errno != EINPROGRESS) {
            perror("connect");
            close(sockfd);
            return 1;
        }

        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(sockfd, &write_fds);
        struct timeval tv = { .tv_sec = 2, .tv_usec = 0 };

        int select_res = select(sockfd + 1, NULL, &write_fds, NULL, &tv);
        if (select_res <= 0) {
            fprintf(stderr, "Connection timeout or error\n");
            close(sockfd);
            return 1;
        }

        if (!FD_ISSET(sockfd, &write_fds)) {
            fprintf(stderr, "No file descriptor ready\n");
            close(sockfd);
            return 1;
        }
    }

    int so_error;
    socklen_t len = sizeof(so_error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len) < 0) {
        perror("getsockopt");
        close(sockfd);
        return 1;
    }
    if (so_error != 0) {
        fprintf(stderr, "Connection error: %s\n", strerror(so_error));
        close(sockfd);
        return 1;
    }

    // Set socket back to blocking mode for sending
    flags = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL, flags & ~O_NONBLOCK) < 0) {
        perror("fcntl");
        close(sockfd);
        return 1;
    }

    const char *message = "Hello TCP world!\n";
    ssize_t sent = send(sockfd, message, strlen(message), 0);
    if (sent < 0) {
        perror("send");
        close(sockfd);
        return 1;
    }

    close(sockfd);
    return 0;
}