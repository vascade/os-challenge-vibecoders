#include "common.h"

int setup_server(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Failed to create socket!");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htobe16(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Failed to bind!");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Failed to listen!");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

int accept_client(int server_fd) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
    if (client_fd < 0) {
        perror("Failed to accept connection!");
    }
    return client_fd;  // caller checks for < 0
}