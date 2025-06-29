#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Declare functions for later
void create_file_syscalls(int* array, int param);

int main(int argc, char* argv[]) {
    // if more or less than 2 args, give an error
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <size>\n", argv[0]);
        return 1;
    }

    // Convert input argument to integer
    int param = atoi(argv[1]);
    // if the parameter is less than 0, return an error
    if (param <= 0) {
        fprintf(stderr, "Invalid array size\n");
        return 1;
    }

    // declare array
    int randarray[param];

    // Fill array with random numbers from 0 to 10
    srand((unsigned int) time(NULL));
    for (int i = 0; i < param; i++) {
        randarray[i] = rand() % 10;
    }

    // Call both functions
    create_file_syscalls(randarray, param);

    return 0;
}

// Create file using system calls
void create_file_syscalls(int* array, int param) {

    //create file to write to
    char* filename = "syscall.txt";
    //open the file
    int file_desc = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    //give an error if the file description is less than 0
    if (file_desc < 0) {
        perror("Error opening file");
        return;
    }

    // Write each number as text in the file until the array is fully iterated through
    //set a buffer for writing into the file
    char buffer[12];
    for (int i = 0; i < param; i++) {
        sprintf(buffer, "%d\n", array[i]);
        write(file_desc, buffer, strlen(buffer));
    }

    //close the file
    close(file_desc);
}
