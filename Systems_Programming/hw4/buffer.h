#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>

#define PATH_MAX 4096

typedef struct {
    char src_path[PATH_MAX]; // Source path
    char dest_path[PATH_MAX]; // Destination path
} FileTask;

typedef struct {
    FileTask *tasks;
    int size;
    int count;
    int in;
    int out;
    int done;
    long total_bytes;
    int num_regular;
    int num_fifo;
    int num_directories;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} Buffer;

void buffer_init(Buffer *buffer, int size);
void buffer_destroy(Buffer *buffer);
void buffer_add(Buffer *buffer, FileTask *task);
int buffer_remove(Buffer *buffer, FileTask *task);
void buffer_set_done(Buffer *buffer);
void buffer_add_total_bytes(Buffer *buffer, size_t bytes);
long buffer_get_total_bytes(Buffer *buffer);
void buffer_add_regular(Buffer *buffer);
int buffer_get_num_regular(Buffer *buffer);
void buffer_add_fifo(Buffer *buffer);
int buffer_get_num_fifo(Buffer *buffer);
void buffer_add_directory(Buffer *buffer);
int buffer_get_num_directories(Buffer *buffer);

#endif // BUFFER_H
