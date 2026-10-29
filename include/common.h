#include <stdlib.h> // for standard c features
#include <stdio.h> // for print()
#include <stdint.h> // for uint64_t
#include <inttypes.h> // for PRIu64, the printf format for uint64_t
#include <string.h> // for memcpy() and memset()
#include <unistd.h> // for read() and close()
#include <sys/socket.h>
#include <netinet/in.h> // for the sockaddr_in struct
#include <endian.h> // for the htobe64() and be64toh() function. Linux only.

#define BACKLOG 100
#define MSG_LEN 49

int server(int server_fd, int port);
void receive(int client_fd);
void send_data(int client_fd, int start_num, int end_num, unsigned char *hash[32]);
