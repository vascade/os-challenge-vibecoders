#include <pthread.h>
#include "queue.h"

#define NPRIO 16
static job_t *head[NPRIO + 1], *tail[NPRIO + 1]; // index 1..16
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

void queue_push(job_t *j)
{
    if (j->prio < 1)
    {
        j->prio = 1;
    }
    if (j->prio > NPRIO)
    {
        j->prio = NPRIO;
    }
    j->next = NULL;

    pthread_mutex_lock(&mtx);
    if (tail[j->prio])
    {
        tail[j->prio]->next = j; // FIFO within a level
    }
    else
    {
        head[j->prio] = j;
    }
    tail[j->prio] = j;
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mtx);
}

job_t *queue_pop(void)
{
    pthread_mutex_lock(&mtx);
    for (;;)
    {
        for (int p = NPRIO; p >= 1; p--)
        { // Scans 16 downto 1
            if (head[p])
            {                       // first non-empty list
                job_t *j = head[p]; // head job
                head[p] = j->next;  // fixes head
                if (!head[p])
                    tail[p] = NULL;         // if list is empty, fix tail
                pthread_mutex_unlock(&mtx); // unlcoks
                return j;
            }
        }
        pthread_cond_wait(&cv, &mtx); // nothing found, unlock and sleep
    }
}