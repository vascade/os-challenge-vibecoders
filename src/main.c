#include "common.h"

static void *worker(void *arg)
{
    (void)arg;
    for (;;)
    {
        job_t *j = queue_pop(); // blocks until a job exists
        send_data(j->fd, j->start, j->end, j->hash);
        close(j->fd);
        free(j);
    }
    return NULL;
}
int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    int server_fd = setup_server(port);

    long ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    if (ncpu < 1)
    {
        ncpu = 1;
    }
    for (long i = 0; i < ncpu; i++)
    {
        pthread_t t;
        if (pthread_create(&t, NULL, worker, NULL) != 0)
        {
            perror("Failed to create a worker thread");
            exit(EXIT_FAILURE);
        }
        pthread_detach(t);  
    }
    while (1)
    {
        int client_fd = accept_client(server_fd);
        if (client_fd < 0)
            continue;
        receive(client_fd);
    }
    return 0;
}