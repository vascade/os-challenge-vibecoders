#include "common.h"

void receive(int client_fd)
{
    unsigned char buffer[MSG_LEN];

    // Read exactly one full packet, since TCP may deliver it in pieces
    size_t total = 0;
    while (total < MSG_LEN)
    {
        ssize_t n = read(client_fd, buffer + total, MSG_LEN - total);
        if (n < 0)
        {
            perror("Failed to read data from client!");
            exit(EXIT_FAILURE);
        }
        if (n == 0)
        {
            // Clean close between packets is fine; mid-packet is an error
            if (total != 0)
            {
                fprintf(stderr, "Client closed in the middle of a packet\n");
            }
            close(client_fd);  // no job was created, so nobody will use it. 
            return;
        }
        total += n;
    }


    job_t *j = malloc(sizeof *j);
    if (!j) {
        close(client_fd);
        return;
    }
    uint64_t start_num, end_num;
    memcpy(j->hash, buffer, 32);
    memcpy(&start_num, buffer + 32, 8);
    memcpy(&end_num, buffer + 40, 8);
    j->start = be64toh(start_num);
    j->end = be64toh(end_num);
    j->prio = buffer[48];
    j->fd = client_fd;

    queue_push(j);
    // We have a complete packet, so parse it
    /*
    unsigned char hash[32];
    uint64_t start_num, end_num;

    memcpy(hash, buffer, 32);
    memcpy(&start_num, buffer + 32, 8);
    memcpy(&end_num, buffer + 40, 8);
    start_num = be64toh(start_num);
    end_num = be64toh(end_num);

    // priority = buffer[48]; // use this if you need it for queueing

    send_data(client_fd, start_num, end_num, hash);
    */
}