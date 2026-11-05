#include "common.h"

int main(int argc, char const *argv[]) {
    // Setup the socket
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int client_fd = server(server_fd, port);

    int running = 1;
    while (running) {
        receive(client_fd);
    }

    close(client_fd);
    return 0;
}

// Close the server when our whole program is done.