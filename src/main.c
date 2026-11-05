#include "common.h"

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    int server_fd = setup_server(port);

    while (1) {
        int client_fd = accept_client(server_fd);
        if (client_fd < 0) continue;

        printf("Connected to client\n");
        receive(client_fd);
        close(client_fd);
    }

    close(server_fd);  // unreachable with while (1); fine to drop
    return 0;
}