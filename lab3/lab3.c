#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Create a struct of your choice are write all the necessary functions to set/get its data
// and also a print function that prints all the struct's info on the screen.
// Use the struct and the functions in main.
// Define the struct

//this struct "superstruct" contains an id and what it ate for lunch

struct superstruct {
   int id;
   char lunch[100];
};


//this function setSuperstruct sets the id and lunch of the superstruct

struct superstruct setSuperStruct(struct superstruct* s, int id, const char* lunch) {
    (*s).id = id;                      // set the id of the superstruct
    strcpy((*s).lunch, lunch);         // Needed to use strcopy to copy the lunch string to the struct's lunch array
}

const char* getSuperStructLunch(const struct superstruct* s) {
    return s->lunch; // Return the lunch field
}

int getSuperStructId(const struct superstruct* s) {
    return s->id;
}

//this function prints the contents of the superstruct

void printSuperStruct(const struct superstruct* s) {
    printf("ID: %d, Lunch: %s\n", getSuperStructId(s), getSuperStructLunch(s));
}


   // Write functions to perform the basic mathematical operations (+, -, *, /) between the two numbers. 
   // Then call them in the main function, and print the results of each operation on the screen.
   // I just used integers, which will be iffy for division due to rounding (if x > y) it will return 0
   int add(int x, int y){
      return x + y;
   }

   float divide(int x, int y){
      if (y == 0){
         printf("Cannot divide by Zero");
         return 1;
      }
      return (float) x / y;
   }

   int multiply(int x, int y){
      return x * y;
   }

   int subtract(int x, int y){
      return x - y;
   }

// Print array iterates through each row and column and prints each 
   
void printarray(int x, int y, int array[x][y]) //initialze array of integers, but when calling function call whole array and not just one value
{
   for (int i = 0; i < x; i++)
   {
      for (int j = 0; j < y; j++)
      {
           printf("%d ", array[i][j]);
      }
      printf("\n");
   }
}


// this function sets a random number for each index in the array
void initializearray(int x, int y, int array[x][y])
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            array[i][j] = rand();
        }
    }

}

int main(int argc, char* argv[])
{
   // Prompt the user to enter 2 integer numbers from 1-10 and store them.

   int x,y;

   struct superstruct myStruct;

    setSuperStruct(&myStruct, 101, "Pizza");

   printSuperStruct(&myStruct);

   
   printf("Enter 2 integer numbers each from 1-10: \n");
   scanf("%d %d", &x, &y);

   if (x < 1 || x > 10 || y < 1 || y > 10)   
   {
        printf("Not in the range of values (1-10)\n\n");
        return 1;
   }
   printf("Numbers entered: %d, %d \n\n", x, y);
   

   // Create a 2d array with dimensions based on the user's integers and write a function to
   // initialize it using random numbers. Write a function to print the array's content on the screen.
   // Call the functions in the main function.

   int array[x][y];
   initializearray(x, y, array);
   printf("initialized array!\n\n");
   printarray(x, y, array);
   printf("\n");



   int addition = add(x, y);
   int multiplication = multiply(x, y);
   float division = divide(x, y);
   int subtraction = subtract(x, y);

   printf("Addition: %d \nSubtraction: %d \nDivision: %f \nMultiplication: %d \n", addition, subtraction, division, multiplication);
}
   

   
   
   
   





   // Create a struct of your choice are write all the necessary functions to set/get its data
   // and also a print function that prints all the struct's info on the screen.
   // Use the struct and the functions in main.
   
   
   


   