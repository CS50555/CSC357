#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

//Task 2: odds and evens 
//write two programs: odds and evens
//each should take a single input N (CLIS)
//odd prints 1->N inclusive odds
//evens prints 1->N inclusive evens
//same formatting evens and odds

void odds(int N);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./odds <number>");
        exit(1);
    }
    //convert the integer in the command line argument to a string
    int cli = atoi(argv[1]);
    odds(cli);
}

//Task 2: odds and evens 
//write two programs: odds and evens
//each should take a single input N (CLIS)
//odd prints 1->N inclusive odds
//evens prints 1->N inclusive evens
//same formatting evens and odds

void odds(int N)
{
    for (int i = 1; i <= N; i ++)
            {
                //if odd, print the integer from 1->N then exit()
                if (i%2 != 0)
                {
                    printf("%d\n", i);
                }
            }
}
