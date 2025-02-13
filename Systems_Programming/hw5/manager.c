#define _DEFAULT_SOURCE
#include "manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>

extern volatile sig_atomic_t stop; // Variable to state if the program should stop

void copy_directory(const char *src_directory, const char *dest_directory, Buffer *buffer);

/**
 * @brief This function is responsible for running the manager thread.
 * 
 * @param arg A pointer to the arguments passed to the manager thread.
 * @return void* This function does not return a value.
 */
void *manager_run(void *arg) {
    ManagerArgs *args = (ManagerArgs *)arg;
    char *src_directory = args->src_directory;
    char *dest_directory = args->dest_directory;
    Buffer *buffer = args->buffer;

    copy_directory(src_directory, dest_directory, buffer);

    buffer_set_done(buffer);
    pthread_exit(NULL);
}

/**
 * Copies a directory from the source directory to the destination directory.
 * 
 * @param src_directory The path of the source directory.
 * @param dest_directory The path of the destination directory.
 * @param buffer A pointer to the Buffer object used for storing file tasks.
 */
void copy_directory(const char *src_directory, const char *dest_directory, Buffer *buffer) {
    DIR *src_dir = opendir(src_directory);
    if (!src_dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(src_dir)) != NULL && !stop) { // Stop if the program should stop
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char src_path[PATH_MAX];
        char dest_path[PATH_MAX];
        snprintf(src_path, PATH_MAX, "%s/%s", src_directory, entry->d_name);
        snprintf(dest_path, PATH_MAX, "%s/%s", dest_directory, entry->d_name);

        if (entry->d_type == DT_DIR) {
            // Create the destination subdirectory if it does not exist
            if (mkdir(dest_path, 0755) < 0 && errno != EEXIST) {
                perror("mkdir");
                continue;
            }
            // Recursive call to copy the directory
            buffer_add_directory(buffer);
            copy_directory(src_path, dest_path, buffer);
        } else if (entry->d_type == DT_REG) {
            FileTask task;
            strncpy(task.src_path, src_path, PATH_MAX);
            strncpy(task.dest_path, dest_path, PATH_MAX);

            buffer_add(buffer, &task);
            buffer_add_regular(buffer);
        } else if (entry->d_type == DT_FIFO) {
            FileTask task;
            strncpy(task.src_path, src_path, PATH_MAX);
            strncpy(task.dest_path, dest_path, PATH_MAX);

            buffer_add(buffer, &task);
            buffer_add_fifo(buffer);
        }
    }

    closedir(src_dir);
}
