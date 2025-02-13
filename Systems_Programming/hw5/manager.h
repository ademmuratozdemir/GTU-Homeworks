#ifndef MANAGER_H
#define MANAGER_H

#include "buffer.h"

typedef struct {
    char *src_directory;
    char *dest_directory;
    Buffer *buffer;
} ManagerArgs;

void *manager_run(void *arg);

extern pthread_barrier_t barrier; // Barrier to synchronize the worker threads

#endif 
