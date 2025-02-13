#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <time.h>


#define FIFO1 "fifo1"
#define FIFO2 "fifo2"


pid_t childProcess1, childProcess2;
const int MAX_CHILDREN = 2;
int child_count = 0;


// a signal handler for waiting for the termination of the child processes and prints the exit status of the child processes
void signal_handler(int signum) {
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Child %d terminated\n", pid);
        printf("Child %d exited with status %d\n", pid, WEXITSTATUS(status));
        child_count++;
    }

    if (child_count == MAX_CHILDREN) {
        unlink(FIFO1);
        unlink(FIFO2);
    }
}





int main(int argc,char *argv[]){
    
    pid_t parent_pid = getpid();
    printf("Parent process ID: %d\n", parent_pid);
    
    if(argc != 2){
        printf("Usage: ./test <positive integer>\nExitting...\n ");
        exit(1);
    }
    if(atoi(argv[1])<1){
        printf("Usage: ./test <positive integer>\nExitting...\n");
        exit(1);
    }
    int arraySize = atoi(argv[1]);
    int randomNumbers[arraySize];

    
    // Generate random numbers
    srand(time(NULL));
    printf("Random numbers array:\n");
    for (int i = 0; i < arraySize; i++) {
        randomNumbers[i] = rand() % 20 +1; // Generate random numbers between 1 and 20
        printf("%d ", randomNumbers[i]);
    }

    // Create FIFOs
    if (mkfifo(FIFO1, 0666) == -1) {
        fprintf(stderr, "Error creating FIFO1\n");
        return EXIT_FAILURE;
    }
    if (mkfifo(FIFO2, 0666) == -1) {
        fprintf(stderr, "Error creating FIFO2\n");
        return EXIT_FAILURE;
    }
    printf("\nFIFOs created successfully.\n");

    
    // Fork first child process
    pid_t childProcess1 = fork();
    if (childProcess1 == -1) {
        fprintf(stderr, "Error forking first child process\n");
        return EXIT_FAILURE;
    } else if (childProcess1 == 0) {
        // Child process 1
        sleep(10);
        int fd1 = open(FIFO1, O_RDONLY);
        if (fd1 == -1) {
            fprintf(stderr, "Error opening FIFO1 for reading\n");
            exit(EXIT_FAILURE);
        }
        
        // Read random numbers from FIFO1 and calculate the sum of the random numbers 
        int numCome;
        int sum=0;
         for (int i = 0; i < arraySize; i++) {
            
            if (read(fd1, &numCome, sizeof(int)) == -1) {
                fprintf(stderr, "Error reading random numbers from FIFO1\n");
                close(fd1);
                exit(EXIT_FAILURE);
            }
            sum += numCome;
        }

        
        printf("Child 1 : Sum of random numbers: %d\n", sum);

        // Send sum to FIFO2
        int fd2 = open(FIFO2, O_WRONLY);
        if (fd2 == -1) {
            fprintf(stderr, "Error opening FIFO2 for writing\n");
            close(fd2);
            exit(EXIT_FAILURE);
        }
        if(write(fd2, &sum, sizeof(sum)) == -1) {
            fprintf(stderr, "Error writing sum to FIFO2\n");
            close(fd1);
            close(fd2);
            exit(EXIT_FAILURE);
        }else{
            printf("Child 1 : Sum sent to FIFO2: %d\n",sum);
        }
        
        if(close(fd1) == -1) {
            fprintf(stderr, "Error closing FIFO1\n");
            exit(EXIT_FAILURE);
        }
        if(close(fd2) == -1) {
            fprintf(stderr, "Error closing FIFO2\n");
            exit(EXIT_FAILURE);
        }
        return EXIT_SUCCESS;
    
    }

    // Fork second child process
    pid_t childProcess2 = fork();
    if (childProcess2 == -1) {
        fprintf(stderr, "Error forking second child process\n");
        return EXIT_FAILURE;
    } else if (childProcess2 == 0) {
        // Child process 2
        
        sleep(10);
        int fd2 = open(FIFO2, O_RDONLY);
        if (fd2 == -1) {
            fprintf(stderr, "Error opening FIFO2 for reading\n");
            exit(EXIT_FAILURE);
        }
        
        // Read random numbers from FIFO2 and calculate the product of the random numbers
        int numCome;
        int product=1;
         for (int i = 0; i < arraySize; i++) {
            
            if (read(fd2, &numCome, sizeof(int)) == -1) {
                fprintf(stderr, "Error reading random numbers from FIFO1\n");
                close(fd2);
                exit(EXIT_FAILURE);
            }
            product *= numCome;
        }
        printf("Child 2 : Multiplication of random numbers: %d\n", product);
        
        // Wait for sending the sum to FIFO2 to provide synchronization
        for(int i=0;i<100;i++)
        {

        }
        
        
        // Read command from FIFO2
        char command[8];
        
        for(int i=0;i<8;i++){
            if (read(fd2, &command[i], sizeof(char)) == -1) {
                fprintf(stderr, "Error reading command from FIFO2\n");
                close(fd2);
                exit(EXIT_FAILURE);
            }
        }
        if(strcmp(command, "multiply") != 0) {
            fprintf(stderr, "Invalid command received from FIFO2\n");
            close(fd2);
            exit(EXIT_FAILURE);
        }
        printf("Child 2 : Multiplication command received.It is %s\n",command);

        // Wait for sending the sum to FIFO2 to provide synchronization
        for(int i=0;i<100;i++)
        {

        }
        
        
        // Read sum from FIFO2
        int sum;
        if (read(fd2, &sum, sizeof(sum)) == -1) {
            fprintf(stderr, "Error reading sum from FIFO2\n");
            close(fd2);
            exit(EXIT_FAILURE);
        }
        printf("Child 2 : Sum received from FIFO2: %d\n", sum);


        printf("Child 2 : Sum of sum and multiplication: %d\n",sum+product);
        if(close(fd2) == -1) {
            fprintf(stderr, "Error closing FIFO2\n");
            exit(EXIT_FAILURE);
        }

        return EXIT_SUCCESS;
        
    }

    // Back to parent process and open FIFOs for writing
    int fd1 = open(FIFO1, O_WRONLY);
    int fd2 = open(FIFO2, O_WRONLY);
    if (fd1 == -1 || fd2 == -1) {
        fprintf(stderr, "Error opening FIFOs for writing\n");
        return EXIT_FAILURE;
    }

      // Send array of random numbers to FIFO1
    if (write(fd1, randomNumbers, arraySize * sizeof(int)) == -1) {
        fprintf(stderr, "Error writing to FIFO1\n");
        return EXIT_FAILURE;
    }else{
        printf("Random numbers array sent to FIFO1.\n");
    }
    // Send array of random numbers to FIFO2
    if (write(fd2, randomNumbers, arraySize * sizeof(int)) == -1) {
        fprintf(stderr, "Error writing to FIFO2\n");
        return EXIT_FAILURE;
    }else{
        printf("Random numbers array sent to FIFO2.\n");
    }
    
    // Send command to FIFO2
    char *command = "multiply";
    if (write(fd2, command, sizeof(command)) == -1) {
        fprintf(stderr, "Error writing to FIFO2\n");
        return EXIT_FAILURE;
    }

    if(close(fd1) == -1) {
        fprintf(stderr, "Error closing FIFO1\n");
        return EXIT_FAILURE;
    }
    if(close(fd2) == -1) {
        fprintf(stderr, "Error closing FIFO2\n");
        return EXIT_FAILURE;
    }

    // Wait for child processes to terminate
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        printf("sigaction");
        return EXIT_FAILURE;
    }
    
    // Parent process loop
    while (child_count < 2) {
        printf("Proceeding...\n");
        sleep(2);
    }

    

    
    return EXIT_SUCCESS;

}