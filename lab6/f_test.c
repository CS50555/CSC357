#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

//Task 1: Fork
// take an integer, N as a CLI
//fork a child process
//child must print odd numbers from 1 to N (inclusive) then exit()
//parent must print even numbers from 1 to N (inclusive)
//parent should wait for child process to terminate
// for odd numbers, use %d\n, for even numbers, use \t%d\n

void f_test(int N);

int main(int argc, char* argv[])
{
    int cli = atoi(argv[1]);
    f_test(cli);
}

//Task 1: Fork
// take an integer, N as a CLI
//fork a child process
//child must print odd numbers from 1 to N (inclusive) then exit()
//parent must print even numbers from 1 to N (inclusive)
//parent should wait for child process to terminate
// for odd numbers, use %d\n, for even numbers, use \t%d\n
void f_test(int N)
{
    pid_t spawnid = 0;

    //creates child process
    spawnid = fork();

    switch(spawnid)
    {
        case -1:
            printf("Fork failed.\n");
            exit(1);
        case 0:
            for (int i = 1; i <= N; i ++)
            {
                //if odd, print the integer from 1->N then exit()
                if (i%2 != 0)
                {
                    printf("%d\n", i);
                }
            }
            exit(0);
            printf("This is the child\n");
        default:
            //wait for child to terminate before the parent runs
            wait(NULL);
            for (int i = 1; i <= N; i++)
            {
                //if even, print the integer form 1->N
                if (i%2 == 0)
                {
                    printf("\t%d\n", i);
                }
            }

    }

}
