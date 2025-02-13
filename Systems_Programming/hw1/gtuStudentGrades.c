#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 1000
#define MAX_TOKENS 100

typedef struct {
    char name[MAX_LINE_LENGTH];
    char grade[MAX_LINE_LENGTH];
} StudentGrade;

void logCompletion(const char *taskName);

// a function to create file
void createFile(const char *filename) {
    
    logCompletion("createFile");
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd != -1) {
            close(fd);
            printf("File %s created successfully.\n", filename);
            exit(EXIT_SUCCESS);
        } else {
            perror("Error creating file");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d.\n\n", WEXITSTATUS(status));
        } else {
            printf("Child process terminated abnormally.\n");
        }
    }
}

// a function to add student grade to file
void addStudentGradeToFile(const char *filename, char *tokens[], int token_count) {
    
    logCompletion("addStudentGradeToFile");
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        
        char *grade = tokens[token_count - 1];
        char studentName[1000]; // Assuming maximum length of student name as 100 characters
        studentName[0] = '\0'; // Initialize studentName as an empty string

        // Concatenate student name tokens with spaces
        for (int i = 2; i < token_count - 1; i++) {
            strcat(studentName, tokens[i]);
            if (i != token_count - 2) {
                strcat(studentName, " ");
            }
        }

        int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd != -1) {
            // Construct the string to be written
            char buffer[256]; // Adjust buffer size as needed
            int n = snprintf(buffer, sizeof(buffer), "%s %s\n", studentName, grade);

            // Write the string to the file
            if (write(fd, buffer, n) != n) {
                perror("Error writing to file");
                close(fd);
                exit(EXIT_FAILURE);
            }

            close(fd);
            printf("Student %s's grade %s added successfully.\n", studentName, grade);
            exit(EXIT_SUCCESS);
        } else {
            perror("Error opening file for appending");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d.\n\n", WEXITSTATUS(status));
        } else {
            printf("Child process terminated abnormally.\n");
        }
    }
}

// a function to search student grade
void searchStudentGrade(const char *filename, char *tokens[],int token_count) {
    
    logCompletion("searchStudentGrade");
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        int fd = open(filename, O_RDONLY);
        if (fd != -1) {
            char line[MAX_LINE_LENGTH];
            char name[MAX_LINE_LENGTH];
            char buffer[MAX_LINE_LENGTH];
            char studentName[100] ;// Assuming maximum length of student name as 100 characters
            studentName[0] = '\0'; // Initialize studentName as an empty string

            // Concatenate student name tokens with spaces
            if(token_count==3){
                strcat(studentName, tokens[2]);
            }
            else{
                for (int i = 2; i < token_count; i++) {
                    strcat(studentName, tokens[i]);
                    if (i < token_count - 1) {
                        strcat(studentName, " ");
                    }
                }
            }
            int found = 0;
            ssize_t bytesRead;
            
           
            while ((bytesRead = read(fd, buffer, MAX_LINE_LENGTH)) > 0) {
                // Split buffer into lines
                char *token = strtok(buffer, "\n");
                while (token != NULL) {
                    // Extract the name from the token
                    sscanf(token, "%s", name);
                    // Find the last whitespace in the token
                    char *lastSpace = strrchr(token, ' ');
                    if (lastSpace != NULL) {
                        // Calculate the length of the name part
                        size_t nameLength = lastSpace - token;
                        // Copy the name part to the name variable
                        strncpy(name, token, nameLength);
                        name[nameLength] = '\0'; // Null-terminate the name string
                    }
                    // Compare the extracted name with the search name
                    if (strcmp(name, studentName) == 0) {
                        printf("Student Name: %s\n", name);
                        printf("Grade: %s\n", lastSpace + 1);
                        found = 1;
                        break;
                    }
                    token = strtok(NULL, "\n");
                }
                if (found == 1) {
                    break; // If found, exit outer loop
                }
            }
            
        
            if (found == 0) {
                printf("Student not found\n");
            }
            close(fd);
            exit(EXIT_SUCCESS);
        } else {
            perror("Error opening file for reading");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status)) {
            printf("Child process terminated abnormally.\n");
        }
    }
}



int compareByNameAscending(const void *a, const void *b) {
    const char *nameA = ((const StudentGrade *)a)->name;
    const char *nameB = ((const StudentGrade *)b)->name;
    while (*nameA && *nameB) {
        int result = tolower(*nameA) - tolower(*nameB);
        if (result != 0) {
            return result;
        }
        nameA++;
        nameB++;
    }
    return *nameA - *nameB;
}

int compareByNameDescending(const void *a, const void *b) {
    return compareByNameAscending(b, a); // Reverse order
}

int compareByGradeAscending(const void *a, const void *b) {
    return strcmp(((StudentGrade*)a)->grade, ((StudentGrade*)b)->grade);
}

int compareByGradeDescending(const void *a, const void *b) {
    return strcmp(((StudentGrade*)b)->grade, ((StudentGrade*)a)->grade);
}


// a function to sort all students
void sortAll(const char *filename) {
    
    logCompletion("sortAll");
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        int fd = open(filename, O_RDONLY);
        if (fd != -1) {
            char buffer[MAX_LINE_LENGTH];
            StudentGrade grades[100];
            int count = 0;
            int sortType;

            ssize_t bytesRead;
            while ((bytesRead = read(fd, buffer, MAX_LINE_LENGTH)) > 0 && count < 100) {
                // Split buffer into lines
                char *token = strtok(buffer, "\n");
                while (token != NULL) {
                    char *lastSpace = strrchr(token, ' ');
                    if (lastSpace != NULL) {
                        *lastSpace = '\0';
                        strcpy(grades[count].name, token);
                        strcpy(grades[count].grade, lastSpace + 1);
                        count++;
                    }
                    token = strtok(NULL, "\n");
                }
            }
            printf("Enter 1 for ascending order by name\n");
            printf("Enter 2 for descending order by name\n");
            printf("Enter 3 for ascending order by grade\n");
            printf("Enter 4 for descending order by grade\n");
            scanf("%d", &sortType);

            switch (sortType) {
                case 1:
                    qsort(grades, count, sizeof(StudentGrade), compareByNameAscending);
                    break;
                case 2:
                    qsort(grades, count, sizeof(StudentGrade), compareByNameDescending);
                    break;
                case 3:
                    qsort(grades, count, sizeof(StudentGrade), compareByGradeAscending);
                    break;
                case 4:
                    qsort(grades, count, sizeof(StudentGrade), compareByGradeDescending);
                    break;
                default:
                    printf("Invalid sort type.\n");
                    return;
            }
            printf("\n");
            for (int i = 0; i < count; i++) {
                printf("Student: %-20s Grade: %s\n", grades[i].name, grades[i].grade);
            }

            close(fd);
            exit(EXIT_SUCCESS);
        } else {
            perror("Error opening file for reading");
            exit(EXIT_FAILURE);
        }
    } else { // Parent process
        wait(NULL); // Wait for child to finish
    }
}


// a function to show all students
void showAll(const char *filename) {
    
    logCompletion("showAll");
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        int fd = open(filename, O_RDONLY);
        if (fd != -1) {
            char buffer[MAX_LINE_LENGTH];
            ssize_t bytesRead;
            printf("\nStudent Grades:\n\n");
            while ((bytesRead = read(fd, buffer, MAX_LINE_LENGTH)) > 0) {
                write(STDOUT_FILENO, buffer, bytesRead);
            }
            close(fd);
            exit(EXIT_SUCCESS);
        } else {
            perror("Error opening file for reading");
            exit(EXIT_FAILURE);
        }
    } else { // Parent process
        wait(NULL); // Wait for child to finish
    }
}

// a function to list first 5 students
void listGrades(const char *filename) {
    
    logCompletion("listGrades");
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        int fd = open(filename, O_RDONLY);
        if (fd != -1) {
            char buffer[MAX_LINE_LENGTH];
            int count = 0;
            int NUM_LINES=5;
            ssize_t bytesRead;
            printf("\nFirst %d Student Grades:\n\n", NUM_LINES);
            while ((bytesRead = read(fd, buffer, MAX_LINE_LENGTH)) > 0) {
                int i = 0;
                while (i < bytesRead) {
                    if (buffer[i] == '\n') {
                        count++;
                        if (count == NUM_LINES) {
                            buffer[i] = '\0'; // Replace newline character with null terminator
                            printf("%s\n", buffer);
                            close(fd);
                            exit(EXIT_SUCCESS);
                        }
                    }
                    i++;
                }
                write(STDOUT_FILENO, buffer, bytesRead);
            }
            close(fd);
            exit(EXIT_SUCCESS);
        } else {
            perror("Error opening file for reading");
            exit(EXIT_FAILURE);
        }
    } else { // Parent process
        wait(NULL); // Wait for child to finish
    }
}

// a function to list students by page number
void listSome(int numPerPage, int pageNumber, const char *filename) {
    
    logCompletion("listSome");
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        int fd = open(filename, O_RDONLY);
        if (fd != -1) {
            char buffer[MAX_LINE_LENGTH];
            int count = 0;
            int start = (pageNumber - 1) * numPerPage;
            int end = pageNumber * numPerPage;
            int currentPage = 1;

            printf("\nPage %d:\n\n", pageNumber);
            ssize_t bytesRead;
            while ((bytesRead = read(fd, buffer, MAX_LINE_LENGTH)) > 0) {
                for (int i = 0; i < bytesRead; i++) {
                    if (count >= start && count < end) {
                        putchar(buffer[i]);
                    }
                    if (buffer[i] == '\n') {
                        count++;
                    }
                    if (count == end) {
                        close(fd);
                        exit(EXIT_SUCCESS);
                    }
                }
            }
            close(fd);
            exit(EXIT_SUCCESS);
        } else {
            perror("Error opening file for reading");
            exit(EXIT_FAILURE);
        }
    } else { // Parent process
        wait(NULL); // Wait for child to finish
    }
}


// a function to print usage
void printUsage() {
    
    logCompletion("printUsage");
    printf("\nUSAGE SECTION: \nCommands: \n");
    printf("\tgtuStudentGrades <filename>    (for creating a new empty .txt file)\n");
    printf("\taddStudentGrade <filename> <student_name> <grade>   (for adding student and its grade to .txt file you created)\n");
    printf("\tsearchStudent <filename> <student_name>   (for search a specific student you entered in the .txt file you created)\n");
    printf("\tsortAll <filename> <sort_type>    (for sorting students and their grades .n the .txt file)\n");
    printf("\tshowAll <filename>   (for showing all students in the .txt file)\n");
    printf("\tlistGrades <filename> <num_entries>   (for listing first 5 students in the .txt file\n");
    printf("\tlistSome <num_per_page> <page_number> <filename>   (for listing students by you entered page number in the .txt file\n");
    printf("\texit   (for exitting the program\n\n");
}


// a function to log completion
void logCompletion(const char *taskName) {
    int log_fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd != -1) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char log_msg[MAX_LINE_LENGTH];
        int len = snprintf(log_msg, MAX_LINE_LENGTH, "[%04d-%02d-%02d %02d:%02d:%02d] Task '%s' completed.\n",
                            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                            t->tm_hour, t->tm_min, t->tm_sec, taskName);
        write(log_fd, log_msg, len);
        close(log_fd);
    } else {
        perror("Error opening log file for writing");
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char *argv[]) {

    printf("Welcome to the student grade program\n");

    while(1){
        

        printf("\nEnter  gtuStudentGrades  for usage\n\n");

        char command[MAX_LINE_LENGTH];
        fgets(command, MAX_LINE_LENGTH, stdin);
        char *tokens[MAX_TOKENS];
        char *str;
        int token_count = 0;
        str= strtok(command," \n");
        while (str != NULL && token_count < MAX_TOKENS) {
            tokens[token_count] = str; // Store the token pointer
            token_count++;

            // Move to the next token
            str = strtok(NULL, " \n");
        }

        if((strcmp(tokens[0],"gtuStudentGrades")==0) && token_count==1){
            printUsage();
            continue;
        }

        if(strcmp(tokens[0], "gtuStudentGrades") == 0){
            if(token_count!=2){
                printf("Invalid command\n");
                continue;
            }
            createFile(tokens[1]);
        }
        else if(strcmp(tokens[0], "addStudentGrade") == 0){
           if(token_count>13){
                printf("Invalid command\n");
                continue;
            }
            addStudentGradeToFile(tokens[1], tokens, token_count);
           
        }
        else if(strcmp(tokens[0], "searchStudent") == 0){
            if(token_count>12){
                printf("Invalid command\n");
                continue;
            }
            searchStudentGrade(tokens[1], tokens, token_count);
        }
        else if(strcmp(tokens[0], "sortAll") == 0){
            sortAll(tokens[1]);
        }
        else if(strcmp(tokens[0], "showAll") == 0){
            showAll(tokens[1]);
        }
        else if(strcmp(tokens[0], "listGrades") == 0){
            listGrades(tokens[1]);
        }
        else if(strcmp(tokens[0], "listSome") == 0){
            listSome(atoi(tokens[1]), atoi(tokens[2]), tokens[3]);
        }
        else if(strcmp(tokens[0], "exit") == 0){
            printf("Exiting...\n");
            exit(EXIT_SUCCESS);
        }
        else{
            printf("Invalid command\n");
            
        }

        
        

        
    }
}