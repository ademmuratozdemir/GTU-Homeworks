#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Initializes a buffer with the given size.
 *
 * @param buffer The buffer to initialize.
 * @param size The size of the buffer.
 */
void buffer_init(Buffer *buffer, int size) {
    buffer->tasks = malloc(size * sizeof(FileTask));
    buffer->size = size;
    buffer->count = 0;
    buffer->in = 0;
    buffer->out = 0;
    buffer->done = 0;
    buffer->total_bytes = 0;
    buffer->num_regular = 0;
    buffer->num_fifo = 0;
    buffer->num_directories = 0;
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->not_empty, NULL);
    pthread_cond_init(&buffer->not_full, NULL);
}

/**
 * @brief Destroys a buffer and releases associated resources.
 *
 * This function frees the memory allocated for the tasks array in the buffer,
 * destroys the mutex used for synchronization, and destroys the condition variables
 * used for signaling when the buffer is not empty or not full.
 *
 * @param buffer Pointer to the buffer to be destroyed.
 */
void buffer_destroy(Buffer *buffer) {
    free(buffer->tasks);
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->not_empty);
    pthread_cond_destroy(&buffer->not_full);
}

/**
 * Adds a FileTask to the buffer.
 *
 * This function adds a FileTask to the buffer. If the buffer is full, the function will wait until there is space available.
 *
 * @param buffer The buffer to add the task to.
 * @param task The FileTask to add to the buffer.
 */
void buffer_add(Buffer *buffer, FileTask *task) {
    pthread_mutex_lock(&buffer->mutex);
    while (buffer->count == buffer->size) {
        pthread_cond_wait(&buffer->not_full, &buffer->mutex);
    }
    buffer->tasks[buffer->in] = *task;
    buffer->in = (buffer->in + 1) % buffer->size;
    buffer->count++;
    pthread_cond_signal(&buffer->not_empty);
    pthread_mutex_unlock(&buffer->mutex);
}

/**
 * Removes a task from the buffer.
 * 
 * @param buffer The buffer from which to remove the task.
 * @param task   A pointer to a FileTask struct to store the removed task.
 * @return       Returns 1 if a task was successfully removed, 0 otherwise.
 */
int buffer_remove(Buffer *buffer, FileTask *task) {
    pthread_mutex_lock(&buffer->mutex);
    while (buffer->count == 0 && !buffer->done) {
        pthread_cond_wait(&buffer->not_empty, &buffer->mutex);
    }
    if (buffer->count == 0 && buffer->done) {
        pthread_mutex_unlock(&buffer->mutex);
        return 0;
    }
    *task = buffer->tasks[buffer->out];
    buffer->out = (buffer->out + 1) % buffer->size;
    buffer->count--;
    pthread_cond_signal(&buffer->not_full);
    pthread_mutex_unlock(&buffer->mutex);
    return 1;
}

/**
 * Sets the 'done' flag of the given buffer to 1, indicating that the buffer is done.
 * This function is thread-safe.
 *
 * @param buffer A pointer to the Buffer struct.
 */
void buffer_set_done(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    buffer->done = 1;
    pthread_cond_broadcast(&buffer->not_empty);
    pthread_mutex_unlock(&buffer->mutex);
}

/**
 * Adds the specified number of bytes to the total_bytes variable of the given buffer.
 * This function is thread-safe and uses a mutex to ensure exclusive access to the buffer.
 *
 * @param buffer The buffer to add bytes to.
 * @param bytes The number of bytes to add.
 */
void buffer_add_total_bytes(Buffer *buffer, size_t bytes) {
    pthread_mutex_lock(&buffer->mutex);
    buffer->total_bytes += bytes;
    pthread_mutex_unlock(&buffer->mutex);
}

/**
 * Retrieves the total number of bytes in the buffer.
 *
 * @param buffer The buffer object.
 * @return The total number of bytes in the buffer.
 */
long buffer_get_total_bytes(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    long total_bytes = buffer->total_bytes;
    pthread_mutex_unlock(&buffer->mutex);
    return total_bytes;
}

/**
 * Adds a regular item to the buffer.
 *
 * This function locks the buffer mutex, increments the count of regular items in the buffer,
 * and then unlocks the mutex.
 *
 * @param buffer A pointer to the Buffer struct.
 */
void buffer_add_regular(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    buffer->num_regular++;
    pthread_mutex_unlock(&buffer->mutex);
}

/**
 * Retrieves the number of regular elements in the buffer.
 *
 * This function locks the buffer's mutex, retrieves the number of regular elements,
 * and then unlocks the mutex before returning the result.
 *
 * @param buffer A pointer to the Buffer struct.
 * @return The number of regular elements in the buffer.
 */
int buffer_get_num_regular(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    int num_regular = buffer->num_regular;
    pthread_mutex_unlock(&buffer->mutex);
    return num_regular;
}

/**
 * Adds an element to the buffer using the First-In-First-Out (FIFO) strategy.
 * This function increments the `num_fifo` variable of the buffer and ensures thread safety
 * by acquiring and releasing the buffer's mutex lock.
 *
 * @param buffer A pointer to the Buffer structure.
 */
void buffer_add_fifo(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    buffer->num_fifo++;
    pthread_mutex_unlock(&buffer->mutex);
}

/**
 * Retrieves the number of elements in the FIFO buffer.
 *
 * This function acquires the lock on the buffer's mutex, retrieves the number of elements
 * in the FIFO buffer, and then releases the lock. The retrieved number of elements is returned.
 *
 * @param buffer A pointer to the Buffer structure.
 * @return The number of elements in the FIFO buffer.
 */
int buffer_get_num_fifo(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    int num_fifo = buffer->num_fifo;
    pthread_mutex_unlock(&buffer->mutex);
    return num_fifo;
}

/**
 * Adds a directory to the buffer.
 * 
 * This function is used to add a directory to the buffer. It increments the
 * number of directories in the buffer by one.
 * 
 * @param buffer A pointer to the Buffer struct.
 */
void buffer_add_directory(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    buffer->num_directories++;
    pthread_mutex_unlock(&buffer->mutex);
}

/**
 * Retrieves the number of directories stored in the buffer.
 *
 * @param buffer The buffer object.
 * @return The number of directories in the buffer.
 */
int buffer_get_num_directories(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
    int num_directories = buffer->num_directories;
    pthread_mutex_unlock(&buffer->mutex);
    return num_directories;
}
