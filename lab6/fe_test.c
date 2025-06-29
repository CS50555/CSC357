#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

//Task 3: exec
//write a function called fe_test
//similar to f_test, but use exec system calls on task 2 functions
//parent should fork two children processes
//one child will exec the odds program
//other child will exec the evens program
//parent should wait until both children terminate 

void fe_test(int N);

int main(int argc, char* argv[])
{
    //int cli = atoi(argv[1]);
    int cli = 10;
    fe_test(cli);
}

//Task 3: exec
//write a function called fe_test
//similar to f_test, but use exec system calls on task 2 functions
//parent should fork two children processes
//one child will exec the odds program
//other child will exec the evens program
//parent should wait until both children terminate 

void fe_test(int N)
{
    //create two children
    pid_t spawnid1 = 0;
    spawnid1 = fork();
    //if spawnid is negative print error message that fork failed
    if (spawnid1 < 0) 
    {
        perror("Fork failed");
        exit(1);
    }
    //execute child 1
    else if (spawnid1 == 0)
    {
        //set a buffer for input number from command line
        char inputnum[10]; 
        sprintf(inputnum, "%d", N);
        //execute the odds functoin
        execl("./odds", "odds", inputnum, NULL);

        //print an error if odds does not run
        perror("execl failed for odds");
        exit(1);
    }


    //create two children
    pid_t spawnid2 = 0;
    spawnid2 = fork();
    
    //if spawnid2 is negative print error message that fork failed
    if (spawnid2 < 0) 
    {
        perror("Fork failed");
        exit(1);
    }
    //execute child 1
    else if (spawnid2 == 0)
    {
        //create a buffer
        char inputnum[10]; 
        //reformat the user inputted integer as a string
        sprintf(inputnum, "%d", N);
        //execute evens
        execl("./evens", "evens", inputnum, NULL);

        perror("execl failed for evens");
        exit(1);
    }

    //wait for both child processes to run

    waitpid(spawnid1, NULL, 0);
    waitpid(spawnid2, NULL, 0);



}