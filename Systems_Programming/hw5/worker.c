#define _DEFAULT_SOURCE
#include "worker.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

extern volatile sig_atomic_t stop; // Variable to state if the program should stop

/**
 * This function is the entry point for the worker thread.
 * It reads tasks from the buffer, copies files from source path to destination path,
 * and updates the total bytes copied in the buffer.
 *
 * @param arg A pointer to the WorkerArgs struct containing the buffer and barrier.
 * @return void* This function does not return a value.
 */
void *worker_run(void *arg) {
    WorkerArgs *args = (WorkerArgs *)arg;
    Buffer *buffer = args->buffer;
    pthread_barrier_t *barrier = args->barrier;

    while (1) {
        FileTask task;
        if (!buffer_remove(buffer, &task)) {
            break;
        }

        int src_fd = open(task.src_path, O_RDONLY);
        if (src_fd < 0) {
            perror("open src");
            continue;
        }

        int dest_fd = open(task.dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd < 0) {
            perror("open dest");
            close(src_fd);
            continue;
        }

        char buf[BUFSIZ];
        ssize_t bytes_read, bytes_written;
        while ((bytes_read = read(src_fd, buf, BUFSIZ)) > 0 && !stop) { // Stop if the program should stop
            char *write_ptr = buf;
            ssize_t bytes_left = bytes_read;

            while (bytes_left > 0) {
                bytes_written = write(dest_fd, write_ptr, bytes_left);
                if (bytes_written <= 0) {
                    perror("write");
                    break;
                }
                bytes_left -= bytes_written;
                write_ptr += bytes_written;
            }
            buffer_add_total_bytes(buffer, bytes_read);
        }

        if (bytes_read < 0) {
            perror("read");
        }

        close(src_fd);
        close(dest_fd);

        
    }

    // Barrier wait
    pthread_barrier_wait(barrier);

    pthread_exit(NULL);
}
