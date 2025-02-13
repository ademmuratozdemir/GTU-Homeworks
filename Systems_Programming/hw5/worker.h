#ifndef WORKER_H
#define WORKER_H

#include "buffer.h"

typedef struct {
    Buffer *buffer;
    pthread_barrier_t *barrier;
} WorkerArgs;

void *worker_run(void *arg);

extern pthread_barrier_t barrier; // Barrier to synchronize the worker threads

#endif // WORKER_H
