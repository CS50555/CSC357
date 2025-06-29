#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define FILENAME "input.txt"

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //Get user input
    printf("Enter a string: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

    //save the input string from user into a file
    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    write(fd, buffer, strlen(buffer));
    close(fd);

    //create a fork for child and parent processes
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) 
    { 
        // Child process
        // Close read end of pipe
        close(pipefd[0]); 
        
        // Convert to uppercase
        for (int i = 0; buffer[i]; i++) {
            buffer[i] = toupper(buffer[i]);
        }
        
        // Write to pipe
        write(pipefd[1], buffer, strlen(buffer) + 1);
        close(pipefd[1]);

        // Save uppercase string to file
        fd = open(FILENAME, O_WRONLY | O_TRUNC);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        write(fd, buffer, strlen(buffer));
        close(fd);
        
        exit(EXIT_SUCCESS);
    } 
    else 
    { 
        // Parent process
        // Close write end of pipe
        close(pipefd[1]); 
        
        // Read from pipe
        read(pipefd[0], buffer, BUFFER_SIZE);
        close(pipefd[0]);
        
        // Print the result
        printf("Uppercase string: %s\n", buffer);
    }
    
    return 0;
}
