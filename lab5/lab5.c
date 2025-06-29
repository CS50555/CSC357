#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

//declare function for later use
void create_file_cfunctions(int* array, int param);

// Create file using C functions
int main(int argc, char* argv[]) {

    //check if correct amount of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <size>\n", argv[0]);
        return 1;
    }

    // Convert input argument to integer
    int param = atoi(argv[1]);
    if (param <= 0) {
        fprintf(stderr, "Invalid array size\n");
        return 1;
    }

    // Allocate array
    int randarray[param];

    // Fill array with random numbers from 0 to 10
    srand((unsigned int) time(NULL));
    for (int i = 0; i < param; i++) {
        randarray[i] = rand() % 10;
    }

    //call function
    create_file_cfunctions(randarray, param);

    return 0;
}

void create_file_cfunctions(int* array, int param) {

    //open the file and make it writeable
    FILE* file = fopen("cfunctions.txt", "w");
    if (!file) {
        perror("Could not open cfunctions.txt");
        return;
    }

    // Write each value as a character
    for (int i = 0; i < param; i++) {
        fprintf(file, "%c", array[i]);
    }

    // Close the file
    fclose(file);
}