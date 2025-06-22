#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number_of_chars>\n", argv[0]);
        exit(1);
    }

    int charnum = atoi(argv[1]);
    if (charnum <= 0) {
        fprintf(stderr, "Error: Invalid number of characters.\n");
        exit(1);
    }

    char randomstring[charnum + 2];  // +1 for '\n', +1 for '\0'

    srand(time(NULL)); // Seed the random number generator

    for (int i = 0; i < charnum; i++) 
    {
        if (rand() % 5 == 0) 
        { // 20% chance to be a space
            randomstring[i] = ' ';
        } 
        else 
        { // 80% chance to be an uppercase letter
            randomstring[i] = 'A' + (rand() % 26);
        }
    }

    randomstring[charnum] = '\n'; // Ensure newline at the end
    randomstring[charnum + 1] = '\0'; // Ensure null-termination

    printf("%s", randomstring); // Print the output

    return 0;
}
