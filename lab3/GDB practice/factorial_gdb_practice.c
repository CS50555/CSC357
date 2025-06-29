/* GDB tutorial
   Description: The purpose of this tutorial is to give you some practice using the GDB debugger.
   The program contains 2 mistakes, which you need to discover.
   When you find them, correct them and make a comment about what the error was. Good Luck!!!
 */

#include <stdio.h>
#include <stdlib.h>

int getNumber()
{
	int x = 0;
	printf("Enter a positive integer: ");
	scanf("%d", &x);		//1. user is not prompted to input anything so need to prompt user for input and return that
	return (x);
}

void printResult(int x, int y)
{
	printf("%d! = %d\n", x, y);
}

int main()
{
    int num, i;
    int factorial = 1;
	
	do
	{
		num = getNumber();
	}
	while(num < 0);
	
	for(i = 1; i <= num; i++) //2. need to change from < to <= in order to multiply the correct amount of times
	{
		factorial = factorial * i;
	}
	
	printResult(num, factorial);
    
    return 0;
}


