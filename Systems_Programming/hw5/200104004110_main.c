#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include "manager.h"
#include "worker.h"
#include "buffer.h"

volatile sig_atomic_t stop = 0; // Variable to state if the program should stop . If the program should stop, this variable will be set to 1

// Signal handler for SIGINT
void handle_sigint(int sig) {
    printf("\nSIGINT received. Stopping...\n");
    stop = 1;
}

// Setup signal handling for SIGINT
void setup_signal_handling() {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

// Print usage information
void print_usage() {
    printf("Usage: MWCp <buffer_size> <num_workers> <src_directory> <dest_directory>\n");
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        print_usage();
        return EXIT_FAILURE;
    }

    setup_signal_handling(); // Setup signal handling

    int buffer_size = atoi(argv[1]);
    int num_workers = atoi(argv[2]);
    char *src_directory = argv[3];
    char *dest_directory = argv[4];

    if (buffer_size <= 0 || num_workers <= 0) {
        print_usage();
        return EXIT_FAILURE;
    }

    // Create destination directory if it doesn't exist
    struct stat st = {0};
    if (stat(dest_directory, &st) == -1) {
        if (mkdir(dest_directory, 0755) < 0 && errno != EEXIST) {
            perror("mkdir dest_directory");
            return EXIT_FAILURE;
        }
    }

    Buffer buffer;
    buffer_init(&buffer, buffer_size);

    pthread_t manager_thread;
    pthread_t workers[num_workers];

    // Barrier initialization
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, num_workers + 1); // +1 for the manager thread

    // Start time
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Start manager thread
    ManagerArgs manager_args = {src_directory, dest_directory, &buffer};
    pthread_create(&manager_thread, NULL, manager_run, &manager_args);

    // Start worker threads
    WorkerArgs worker_args = {&buffer, &barrier};
    for (int i = 0; i < num_workers; i++) {
        pthread_create(&workers[i], NULL, worker_run, &worker_args);
    }

    // Wait for manager thread to finish
    pthread_join(manager_thread, NULL);

    // Send termination signal to worker threads
    buffer_set_done(&buffer);

    // Barrier wait for all threads to finish
    pthread_barrier_wait(&barrier);

    // Wait for worker threads to finish
    for (int i = 0; i < num_workers; i++) {
        pthread_join(workers[i], NULL);
    }

    // End time
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long nanoseconds = end_time.tv_nsec - start_time.tv_nsec;
    double elapsed = seconds + nanoseconds * 1e-9;

    long total_bytes = buffer_get_total_bytes(&buffer);
    int num_regular = buffer_get_num_regular(&buffer);
    int num_fifo = buffer_get_num_fifo(&buffer);
    int num_directories = buffer_get_num_directories(&buffer);

    buffer_destroy(&buffer);
    pthread_barrier_destroy(&barrier);

    printf("\n---------------STATISTICS--------------------\n");
    printf("Consumers: %d - Buffer Size: %d\n", num_workers, buffer_size);
    printf("Number of Regular Files: %d\n", num_regular);
    printf("Number of FIFO Files: %d\n", num_fifo);
    printf("Number of Directories: %d\n", num_directories);
    printf("TOTAL BYTES COPIED: %ld\n", total_bytes);
    printf("TOTAL TIME: %02ld:%02ld.%03ld (min:sec.milli) - Elapsed: %.3f seconds\n", seconds / 60, seconds % 60, nanoseconds / 1000000, elapsed);

    return EXIT_SUCCESS;
}
