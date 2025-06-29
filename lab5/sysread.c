#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


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


    //make file syscall.txt so I can read this file later
    char* file = "syscall.txt";
    //set the file descriptor
    int file_desc;
    ssize_t nread;
    //need to make the buffer large enough to handle 8912 bytes read
    char buffer [10000];

    //open the file
    file_desc = open(file, O_RDONLY);

    //file descritor is negative print an error
    if (file_desc < 0)
    {
        fprintf(stderr, "Couldn't open %s\n", file);
    }

    //use the system call read to read the file
    nread = read(file_desc, buffer, param);
}