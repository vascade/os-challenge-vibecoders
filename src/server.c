#include "common.h"

int server(int server_fd, int port) {
    if (server_fd < 0) {
        perror("Failed to create socket!");
        exit(EXIT_FAILURE);
    }

    // Struct with all of the info of our socket
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr)); // Zero it so no leftover garbage in unused fields
    addr.sin_family = AF_INET; // For ipv4 traffic.
    addr.sin_addr.s_addr = INADDR_ANY; // Use all network interfaces.
    addr.sin_port = htobe16(port); // Convert from little endian to big endian, cause thats what it expects.

    // Binding the socket, using the struct
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Failed to bind!");
        exit(EXIT_FAILURE);
    }

    // Listening on the socket
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Failed to listen!");
        exit(EXIT_FAILURE);
    }

    // We keep listening on server_fd, and create a new client_fd on succesful connection.
    // This new client_fd will have its own attributes such as adress and stuff, and we store them in the same struct as before
    int connected = 0;
    while (connected) {
        struct sockaddr_in client_addr; // Setup empty struct that will be filed with client data
        socklen_t client_addr_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len); //Create a new file descriptor for the client and check for errors.

        if (client_fd < 0) {
            perror("Failed to accept connection!");
            exit(EXIT_FAILURE);
        }
        else {
            connected = 1;
            printf("Connected to client");
            return client_fd;
        }
    }
}