#include <stdlib.h> // for standard c features
#include <stdio.h> // for print()
#include <stdint.h> // for uint64_t
#include <inttypes.h> // for PRIu64, the printf format for uint64_t
#include <string.h> // for memcpy() and memset()
#include <unistd.h> // for read() and close()
#include <sys/socket.h>
#include <netinet/in.h> // for the sockaddr_in struct
#include <endian.h> // for the htobe64() and be64toh() function. Linux only.
#include "lonesha256.h"

#define BACKLOG 100
#define MSG_LEN 49

int main(int argc, char const *argv[]) {
    // --- Setup the socket --- ###
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
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

    while (1) {
        struct sockaddr_in client_addr; // Setup empty struct that will be filed with client data
        socklen_t client_addr_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len); //Create a new file descriptor for the client and check for errors.

        if (client_fd < 0) {
            perror("Failed to accept connection!");
            exit(EXIT_FAILURE);
        }

        // Read exactly MSG_LEN bytes, since TCP may deliver them in pieces
        unsigned char buffer[MSG_LEN];
        size_t total = 0;
        while (total < MSG_LEN) {
            // Setup variable n to store what we read from the client
            ssize_t n = read(client_fd, buffer + total, MSG_LEN - total);  // We use ssize_t cause its signed, and if we read -1 there is an error.
            if (n < 0) {
                perror("Failed to read data from client!");
                exit(EXIT_FAILURE);
            }
            if (n == 0) break; // Client closed the connection early
            total += n;
        }

        if (total == MSG_LEN) { // only parse packages with the correct length.
            // Setup vars for splitting the buffer up
            unsigned char hash[32];
            uint64_t start_num, end_num;
            unsigned int priority = buffer[48];

            memcpy(hash, buffer, 32);
            memcpy(&start_num, buffer + 32, 8);
            memcpy(&end_num, buffer + 40, 8);
            start_num = be64toh(start_num); // Big endian from the network -> host byte order
            end_num = be64toh(end_num);

            printf("Buffer size: %zu\n", total);

            printf("Hash: ");
            for (int i = 0; i < 32; i++) printf("%02x", hash[i]);

            printf("\n");
            printf("Start: %" PRIu64 "\n", start_num);
            printf("End: %" PRIu64 "\n", end_num);
            printf("Priority: %u\n", priority);

            // Encrypt and compare.
            unsigned char out[32];
            for (int i = start_num; i <= end_num; i++) {
                if (lonesha256(out, hash, strlen(hash))) {
                    printf("TEST %d: ERROR\n", i);
                }
                else if (memcmp(out, i, sizeof(out))) {
                    printf("TEST %d: FAILED\n", i);
                }
                else {
                    printf("TEST %d: PASSED\n", i);
                }
            }
        }
        // Close the client when we are done with it.
        close(client_fd);
    }

    // Close the server when our whole program is done.
    close(server_fd);

    return 0;
}