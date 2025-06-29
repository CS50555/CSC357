#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUFFER_SIZE 8192  // Set a reasonable maximum size

void main(int argc, char* argv[])
{
        //check if more or less than 2 cmls, give error message if so
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <size>\n", argv[0]);
        return;
    }

    // Convert input argument to integer
    int param = atoi(argv[1]);
    //make sure the param is greater than 0
    if (param <= 0) {
        fprintf(stderr, "Invalid array size\n");
        return;
    }

    FILE *file = fopen("syscall.txt", "r");
    if (file == NULL)
    {
        perror("Cannot open file");
        return;
    }

    
    char buffer[MAX_BUFFER_SIZE];

    size_t bytes_read = fread(buffer, 1, param, file);
    
    
}