#include "common.h"

void send(int *client_fd, int start_num, int end_num) {
    // Encrypt and compare.
    unsigned char out[32];

    for (uint64_t i = start_num; i <= end_num; i++) {
        if (lonesha256(out, &i, sizeof(i))) {
            printf("TEST %" PRIu64 ": ERROR\n", i);
        } else if (memcmp(out, hash, sizeof(out)) == 0) {
            printf("TEST %" PRIu64 ": PASSED\n", i);

            const uint64_t ret_num = htobe64(i);
            write(client_fd, &ret_num, sizeof(ret_num));
            break;
        }
    }
    close(client_fd);
}