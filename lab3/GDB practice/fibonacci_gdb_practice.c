/* GDB tutorial
   Description: The purpose of this tutorial is to give you some practice using the GDB debugger.
   The program contains 3 mistakes, which you need to discover.
   When you find them, correct them and make a comment about what the error was. Good Luck!!!
 */
 
#include <stdio.h>

int getNumber()
{
	int x;
	printf("Enter how many Fibonacci terms you want: ");
	scanf("%d", &x);
	return (x);
}

int main()
{
  int n, first = 0, second = 1, next, c;
 
  n = getNumber();
 
  printf("First %d terms of Fibonacci series are:\n", n);
 
  for (c = 0; c < n; c++) //1. c should be < n not just <= n so it iterates n times not n + 1 times (assuming fibonnaci sequence starts with 0 not 1)
  {
    if (c == 0) 
    {
      next = first; //2. initialze next as first and second so it will guaranteed be 0 and 1 on the first two runs
    }
    else if (c == 1)
    {
      next = second;
    }
    else
    {
      next = first + second; //3. next = first + second term without adding next again
      first = second;
      second = next;
    }
    printf("%d ", next);
  }
 
  printf("\n");
  return 0;
}