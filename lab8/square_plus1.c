#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 32

int main() 
{
    int pipe1[2];
    int pipe2[2]; 
    int pipe3[2];           
    pid_t pid1;
    pid_t pid2;
    char buffer[BUFFER_SIZE];

    //create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe3) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //fork first child
    if ((pid1 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) 
    { // Child 1
        //close the write end of pipe1
        close(pipe1[1]); 
        //close the read end of pipe 2
        close(pipe2[0]); 

        int num;
        //read the input number from the parent
        while (read(pipe1[0], &num, sizeof(num)) > 0)
        {
            //square the input number 
            num *= num; 
            // send squared number to child 2
            write(pipe2[1], &num, sizeof(num)); 
        }
        //close both pipes
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    }

    //create a fork for child 2
    if ((pid2 = fork()) == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0)
    { //child 2
        //close write end of pipe2
        close(pipe2[1]); 
        //close read end of pipe3
        close(pipe3[0]);

        int num = 0;
        //read the number from the first child
        while(read(pipe2[0], &num, sizeof(num)) > 0)
        { 
            // add one to the squared number
            num += 1; 
            //send the added number to the parent
            write(pipe3[1], &num, sizeof(num)); 
        }
        //close the pipes
        close(pipe2[0]);
        close(pipe3[1]);
        exit(0);
    }
    else
    {
        //parent process
        //close read end of pipe1
        close(pipe1[0]); 
        //close read end of pipe2
        close(pipe2[0]);
        //close write end of pipe2 
        close(pipe2[1]); 
        //close write end of pipe3
        close(pipe3[1]); 
    
        printf("Enter integers until ctrl+D (EOF):\n");

        int inputEOF = 0;  // Flag to detect EOF
        
        //iterate through the input and get all of the integers
        while (!inputEOF) 
        {
            if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
            {
                if (feof(stdin))
                {
                    printf("\nEOF reached. Exiting...\n");
                    //close the pipes
                    close(pipe1[1]);
                    close(pipe3[0]);
                    exit(0);
                } else
                {
                    perror("fgets");
                }
            } 
            else
            {
                //convert the string of integers to integers
                int num = atoi(buffer);

                //send the input to child 1
                write(pipe1[1], &num, sizeof(num)); 
                //read the results from child 2
                if (read(pipe3[0], &num, sizeof(num)) > 0)
                {
                    printf("Result: %d\n", num);
                }
            }
        }

        //close the pipes
        close(pipe1[1]);
        close(pipe3[0]);

        // wait for children to finish before exiting
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);

    }

    return 0;
}
