#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#define BUFFER_SIZE 1024

typedef struct {
    int client_id;
    int x;
    int y;
} order;

int sock;

void sigint_handler(int sig) {
    char *cancel_msg = "CANCEL ALL ORDERS";
    send(sock, cancel_msg, strlen(cancel_msg), 0);
    printf(" signal .. cancelling orders.. editing log..\n");
    close(sock);
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

int main(int argc, char *argv[]) {
    pid_t pid;
    // print client pid
    pid = getpid();
    printf("Client PID: %d...\n", pid);
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <serverIP> <portnumber> <numberOfClients> <p> <q>\n", argv[0]);
        return 1;
    }

    char *serverIP = argv[1];
    int port = atoi(argv[2]);
    int numberOfClients = atoi(argv[3]);
    int p = atoi(argv[4]);
    int q = atoi(argv[5]);
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverIP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    setup_signal_handling();

    srand(time(NULL));  // Initialize random seed

    // Allocate memory for orders
    order *orders = malloc(numberOfClients * sizeof(order));
    for (int i = 0; i < numberOfClients; i++) {
        orders[i].client_id = i + 1;
        orders[i].x = rand() % p+1;
        orders[i].y = rand() % q+1;
    }

    if(send(sock, &pid, sizeof(pid), 0) == -1){
        perror("Failed to send pid");
        close(sock);
        free(orders);
        return -1;
    }

    // Send the number of orders first
    if (send(sock, &numberOfClients, sizeof(numberOfClients), 0) == -1) {
        perror("Failed to send number of orders");
        close(sock);
        free(orders);
        return -1;
    }

    // Send all orders at once
    if (send(sock, orders, numberOfClients * sizeof(order), 0) == -1) {
        perror("Failed to send orders");
        close(sock);
        free(orders);
        return -1;
    }

    printf("All orders sent successfully.\n");

    // Wait for server's response
    char buffer[BUFFER_SIZE];
    if (recv(sock, buffer, BUFFER_SIZE, 0) > 0) {
        printf("Server response: %s\n", buffer);
    }

    // Close socket and free resources
    close(sock);
    free(orders);
    return 0;
}
