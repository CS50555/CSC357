#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>


//Task 4: Redirection
//take 2 CLIs 1. name of another program  2. name of file
//use "exec" to run specified program
//redirect exec output to specified file
void to_file(int N, char* program, char* filename);

int main(int argc, char* argv[])
{
    //check if there are enough arguments
    if (argc > 4 || argc < 3)
    {
        printf("Usage: %s ./<program> [optional number] <outputfile>\n", argv[0]);
        exit(1);
    }

    //if trying to execute ./odds or ./evens, need to prompt user for number
    if (argv[1] == "./odds\0" || argv[1] == "./evens\0")
    {
        printf("TRUE TRUE");
        int tempnum = 0;
        scanf("Enter a number: %d\n", &tempnum);
        to_file(tempnum, argv[1], argv[2]);
        return 0;
    }

    //run to_file (works with running fe_test but anything that requires clis is printing an error to the file)
    //needed to pass in 0 as an arbitrary input
    to_file(0, argv[1], argv[2]);
}


void to_file(int N, char* program, char* filename)
{
    //open the file and make it writeable and if there is no file make it createable and truncate it every time
    int filedescriptor = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);

    //if filedescriptor is negative, print an error and exit
    if (filedescriptor < 0)
    {
        perror("Open File Failed");
        exit(1);
    }

    //create a file descriptor to redirect the standard output to the file
    int filedescr1 = dup2(filedescriptor, STDOUT_FILENO);
    //if the new filedescriptor is negative, print an error
    if (filedescr1 < 0)
    {
        perror("Dup2 failed");
        exit(1);
    }

    //close the file

    close(filedescriptor);

    //execute the user inputted program and any cli integer
    execlp(program, program, N, NULL);

    //print an error if the execution failed
    perror("execlp failed");

    //exit with error code one if it does not work
    exit(1);
}
