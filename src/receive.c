#include "common.h"

void receive(int *client_fd) {
    // Read exactly MSG_LEN bytes, since TCP may deliver them in pieces
    unsigned char buffer[MSG_LEN];
    size_t total = 0;
    while (total < MSG_LEN) {
        // Setup variable n to store what we read from the client
        ssize_t n = read(*client_fd, buffer + total, MSG_LEN - total); // We use ssize_t cause its signed, and if we read -1 there is an error.
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

        // Logic for queing based on priority

        send(&client_fd, start_num, end_num);
}