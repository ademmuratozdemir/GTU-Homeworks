#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <poll.h>

#define BUFFER_SIZE 1024
#define INITIAL_QUEUE_SIZE 10
#define MAX_OVEN_CAPACITY 6
#define MAX_DELIVERY_CAPACITY 3
#define NUM_OVEN_APARATUS 3

typedef struct {
    int client_id;
    int x;
    int y;
} order;

typedef struct {
    order* orders;
    int front;
    int rear;
    int size;
    int capacity;
} queue;

typedef struct {
    int id;
    int deliveries_made;
    pthread_mutex_t mutex;
} delivery_person_t;

queue orders_queue;
queue cooked_queue;
delivery_person_t* delivery_persons;
int num_orders;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cooked_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cooked_not_empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t delivery_mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t oven_aparatus;
sem_t oven_capacity;
int num_cooks, num_deliveries, delivery_speed;
int orders_processed = 0;
int orders_delivered = 0;
int all_orders_cooked = 0;
int cancel_all_orders = 0;

int log_file_fd;
int server_sock;

void queue_init(queue* q, int capacity) {
    q->orders = (order*)malloc(capacity * sizeof(order));
    q->capacity = capacity;
    q->front = q->size = 0;
    q->rear = capacity - 1;
}

int is_full(queue* q) {
    return (q->size == q->capacity);
}

int is_empty(queue* q) {
    return (q->size == 0);
}

void enqueue(queue* q, order item) {
    if (!is_full(q)) {
        q->rear = (q->rear + 1) % q->capacity;
        q->orders[q->rear] = item;
        q->size++;
    }
}

order dequeue(queue* q) {
    order item = q->orders[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return item;
}

void queue_free(queue* q) {
    if (q->orders != NULL) {
        free(q->orders);
        q->orders = NULL;
    }
}

void* cook_function(void* arg) {
    int id = *((int*)arg);
    while (1) {
        pthread_mutex_lock(&queue_mutex);
        if (is_empty(&orders_queue) && orders_processed >= num_orders) {
            pthread_mutex_unlock(&queue_mutex);
            return NULL;
        }
        if (is_empty(&orders_queue) || cancel_all_orders) {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        order ord = dequeue(&orders_queue);
        pthread_mutex_unlock(&queue_mutex);

        if (cancel_all_orders) break;

        char buffer[BUFFER_SIZE];
        sprintf(buffer, "Cook %d is preparing order %d\n", id, ord.client_id);
        write(log_file_fd, buffer, strlen(buffer));
        usleep(200000); // Simulate preparation time

        sem_wait(&oven_aparatus);
        sem_wait(&oven_capacity);

        sprintf(buffer, "Cook %d is cooking order %d\n", id, ord.client_id);
        write(log_file_fd, buffer, strlen(buffer));
        usleep(100000); // Simulate cooking time

        sem_post(&oven_aparatus);
        sem_post(&oven_capacity);

        pthread_mutex_lock(&cooked_mutex);
        enqueue(&cooked_queue, ord);
        pthread_cond_signal(&cooked_not_empty);
        pthread_mutex_unlock(&cooked_mutex);

        sprintf(buffer, "Cook %d has finished order %d\n", id, ord.client_id);
        write(log_file_fd, buffer, strlen(buffer));

        pthread_mutex_lock(&queue_mutex);
        orders_processed++;
        if (orders_processed >= num_orders) {
            all_orders_cooked = 1;
            pthread_cond_broadcast(&cooked_not_empty);
            pthread_mutex_unlock(&queue_mutex);
            return NULL;
        }
        pthread_mutex_unlock(&queue_mutex);
    }
    return NULL;
}

void* delivery_function(void* arg) {
    delivery_person_t* dp = (delivery_person_t*)arg;

    while (1) {
        pthread_mutex_lock(&cooked_mutex);
        while (is_empty(&cooked_queue) && !all_orders_cooked && !cancel_all_orders) {
            pthread_cond_wait(&cooked_not_empty, &cooked_mutex);
        }

        if ((is_empty(&cooked_queue) && all_orders_cooked) || cancel_all_orders) {
            pthread_mutex_unlock(&cooked_mutex);
            break;
        }

        if (!is_empty(&cooked_queue)) {
            order ord = dequeue(&cooked_queue);
            pthread_mutex_unlock(&cooked_mutex);

            if (cancel_all_orders) break;

            char buffer[BUFFER_SIZE];
            sprintf(buffer, "Delivery person %d is delivering order %d\n", dp->id, ord.client_id);
            write(log_file_fd, buffer, strlen(buffer));
            
            int delivery_time = (abs(ord.x) + abs(ord.y)) / delivery_speed;
            sleep(delivery_time);  // Simulate delivery time
            
            sprintf(buffer, "Delivery person %d has delivered order %d\n", dp->id, ord.client_id);
            write(log_file_fd, buffer, strlen(buffer));
            dp->deliveries_made++;

            pthread_mutex_lock(&delivery_mutex);
            orders_delivered++;
            if (orders_delivered >= num_orders) {
                pthread_mutex_unlock(&delivery_mutex);
                break;
            }
            pthread_mutex_unlock(&delivery_mutex);
        } else {
            pthread_mutex_unlock(&cooked_mutex);
        }
    }

    return NULL;
}

void* cancel_listener_function(void* arg) {
    int client_sock = *((int*)arg);
    char buffer[BUFFER_SIZE];
    struct pollfd pfd;
    pfd.fd = client_sock;
    pfd.events = POLLIN;

    while (1) {
        int poll_result = poll(&pfd, 1, 1000);  // 1 second timeout

        if (poll_result > 0) {
            int recv_len = recv(client_sock, buffer, BUFFER_SIZE, 0);
            if (recv_len > 0 && strcmp(buffer, "CANCEL ALL ORDERS") == 0) {
                pthread_mutex_lock(&queue_mutex);
                cancel_all_orders = 1;
                pthread_mutex_unlock(&queue_mutex);

                pthread_mutex_lock(&cooked_mutex);
                pthread_cond_broadcast(&cooked_not_empty);
                pthread_mutex_unlock(&cooked_mutex);
                break;
            } else if (recv_len <= 0) {
                // Client disconnected
                break;
            }
        } else if (poll_result == 0) {
            // Timeout, check for cancel_all_orders
            if (cancel_all_orders) {
                break;
            }
        }
    }
    return NULL;
}

void cleanup_resources() {
    sem_destroy(&oven_aparatus);
    sem_destroy(&oven_capacity);

    queue_free(&orders_queue);
    queue_free(&cooked_queue);

    if (delivery_persons) {
        for (int i = 0; i < num_deliveries; i++) {
            pthread_mutex_destroy(&delivery_persons[i].mutex);
        }
        free(delivery_persons);
        delivery_persons = NULL;
    }
}

void sigint_handler(int sig) {
    printf(" .. Upps quiting.. writing log file\n");
    log_file_fd = open("pide_shop_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "> Server is shut down\n");
    write(log_file_fd, buffer, strlen(buffer));
    pthread_mutex_destroy(&queue_mutex);
    pthread_mutex_destroy(&cooked_mutex);
    pthread_mutex_destroy(&delivery_mutex);
    pthread_cond_destroy(&queue_not_empty);
    pthread_cond_destroy(&cooked_not_empty);

    close(server_sock);
    cleanup_resources();
    close(log_file_fd);
    exit(0);
}

void setup_signal_handling() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;  // No flags
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <port> <cook_count> <delivery_count> <delivery_speed>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    num_cooks = atoi(argv[2]);
    num_deliveries = atoi(argv[3]);
    delivery_speed = atoi(argv[4]);

    log_file_fd = open("pide_shop_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_file_fd == -1) {
        perror("Failed to open log file");
        return -1;
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        return -1;
    }

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        return -1;
    }

    if (listen(server_sock, 10) == -1) {
        perror("Listening failed");
        return -1;
    }

    setup_signal_handling();

    while (1) {
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "> PideShop active waiting for connection...\n");
        write(log_file_fd, buffer, strlen(buffer));
        printf("\n>PideShop active waiting for connection...\n");
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock == -1) {
            perror("Accept failed");
            continue;
        }
        // receive pid of client
        pid_t pid;
        if (recv(client_sock, &pid, sizeof(pid), 0) == -1) {
            perror("Failed to receive pid");
            close(client_sock);
            continue;
        }

        sprintf(buffer, "> New customer connected...\n");
        write(log_file_fd, buffer, strlen(buffer));
        printf(">New customer connected... client # %d\n",pid);

        if (recv(client_sock, &num_orders, sizeof(int), 0) <= 0) {
            perror("Failed to receive number of orders");
            close(client_sock);
            continue;
        }

        order* orders = (order*)malloc(num_orders * sizeof(order));
        if (recv(client_sock, orders, num_orders * sizeof(order), 0) <= 0) {
            perror("Failed to receive orders");
            close(client_sock);
            free(orders);
            continue;
        }

        sprintf(buffer, "> %d new customers... Serving\n", num_orders);
        write(log_file_fd, buffer, strlen(buffer));
        printf("> %d new customers... Serving\n", num_orders);

        // Initialize queues
        queue_init(&orders_queue, num_orders);
        queue_init(&cooked_queue, num_orders);

        pthread_mutex_lock(&queue_mutex);
        for (int i = 0; i < num_orders; i++) {
            enqueue(&orders_queue, orders[i]);
        }
        free(orders);
        pthread_mutex_unlock(&queue_mutex);

        sprintf(buffer, "Received and queued %d orders.\n", num_orders);
        write(log_file_fd, buffer, strlen(buffer));

        sem_init(&oven_aparatus, 0, NUM_OVEN_APARATUS);
        sem_init(&oven_capacity, 0, MAX_OVEN_CAPACITY);

        pthread_t cook_threads[num_cooks];
        int cook_ids[num_cooks];
        for (int i = 0; i < num_cooks; i++) {
            cook_ids[i] = i + 1;
            pthread_create(&cook_threads[i], NULL, cook_function, &cook_ids[i]);
        }

        delivery_persons = (delivery_person_t*)malloc(num_deliveries * sizeof(delivery_person_t));
        pthread_t delivery_threads[num_deliveries];
        for (int i = 0; i < num_deliveries; i++) {
            delivery_persons[i].id = i + 1;
            delivery_persons[i].deliveries_made = 0;
            pthread_mutex_init(&delivery_persons[i].mutex, NULL);
            pthread_create(&delivery_threads[i], NULL, delivery_function, &delivery_persons[i]);
        }

        pthread_t cancel_listener_thread;
        pthread_create(&cancel_listener_thread, NULL, cancel_listener_function, &client_sock);

        int cook_threads_active = num_cooks;
        int delivery_threads_active = num_deliveries;

        for (int i = 0; i < num_cooks; i++) {
            pthread_join(cook_threads[i], NULL);
            cook_threads_active--;
        }
        for (int i = 0; i < num_deliveries; i++) {
            pthread_join(delivery_threads[i], NULL);
            delivery_threads_active--;
        }

        // If cook and delivery threads are done, we need to make sure the listener thread is also done
        if (cook_threads_active == 0 && delivery_threads_active == 0) {
            pthread_cancel(cancel_listener_thread); // Cancel the listener thread if everything is done
        }

        pthread_join(cancel_listener_thread, NULL);

        // Cleanup resources after processing each client
        cleanup_resources();

        if (cancel_all_orders) {
            sprintf(buffer, "Client order cancellation received\n");
            printf(">Client order cancellation received\n");
        } else {
            sprintf(buffer, "All orders have been processed and delivered\n");
            printf(">done serving client %d\n",pid);
        }
        
        write(log_file_fd, buffer, strlen(buffer));

        char response[BUFFER_SIZE];
        if (cancel_all_orders) {
            strcpy(response, "Client order cancellation received");
        } else {
            strcpy(response, "All customers served");
        }
        send(client_sock, response, strlen(response), 0);
        close(client_sock);

        // Reset global variables for new client
        orders_processed = 0;
        orders_delivered = 0;
        all_orders_cooked = 0;
        cancel_all_orders = 0;
    }

    close(log_file_fd);
    close(server_sock);
    return 0;
}
