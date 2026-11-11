#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

typedef struct job {
    int fd;
    uint8_t hash[32];
    uint64_t start, end;
    uint8_t prio;
    struct job *next;
} job_t;

void queue_push(job_t *j);
job_t *queue_pop(void);   // blocks until a job exists

#endif