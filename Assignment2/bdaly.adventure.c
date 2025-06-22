#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//define constants for later use with static arrays etc.

#define ROOM_COUNT 7
#define MAX_CONNECTIONS 6
#define MIN_CONNECTIONS 3
#define MAX_TOKENS 10
#define MAX_LENGTH 50
#define MAX_PATH 90

//Need to make a struct for a room object with an enter and exit
//room must have:
//name  (string)
//type  (string)
//connections (array of PIDs or connections to other rooms)
//each room must have a maximum of 6 connections and a minumum of 3 connections

typedef struct Room
{   
    char room_name[20];
    char room_type[20];
    char connections[MAX_CONNECTIONS][20];
    int numconnections; 
}Room;

//need to create 7 different rooms
//need to create a directory called
//need a randomizer helper function to randomize the connections
//cannot have the same number (cannot have a random number the same as an existing number)
//need a connected path from the beginning room to the end

//Declare the helper functions I am using to create the rooms, connect them, write them to files, read the files, and start the game
void createRooms();
void connect_rooms();
void write_room_files();
void start();
char** filereader(char* room_name);

//Declare an array of room structs
//Declare an array to track the path taken by the user
//Declare and initialze the index of the path array so I can append to it later
Room rooms[ROOM_COUNT];
char* path[MAX_PATH];
int path_idx = 0;


//Main function: In this function I create the directory for all of the room files (and get/use the current PID for this), and I create the rooms, connect them, write them to files, and run the game, then exit with a 0 error code
//1. Seed the random number generator
//2. declare an array to hold the directory name 
//3. Get the PID and use snprintf to concatenate the pid to the instructed directory
//4. Make the directory
//5. Create the rooms randomly
//6. Connect the rooms randomly to eachother
//7. write the rooms (name, type and connections) to files
//8. start the game
//9. return with error code 0 to end the game

int main(int argc, char* argv[])
{
    //Seed the random number generator
    srand(time(NULL)); 
    char dirname[50];
    int pid = getpid();
    //snprintf formats the string so I can concatenate the pid to it
    snprintf(dirname, sizeof(dirname), "bdaly.rooms.%d", pid);
    mkdir(dirname, 0777);
    createRooms();
    connect_rooms();
    write_room_files();
    start();
    return 0;

}


//CreateRooms function: in this function, I initilize 10 rooms structs and randomly choose 7 of the 10 choices for the game
//1. hard code 10 room names to choose from
//2. loop through the rooms and assign a random index (choose a random room) to the rooms struct to create that room with that randomized name
//3. ensure each room has no connection by setting the number of connectionsin the struct to 0
//4. copy Start Room, End Room, and Middle Room to the corresponding rooms
void createRooms()
{
    //hardcode 10 different room names 
    char* room_names[] = {"kitchen", "boiler", "garage", "Thorox", "Attic", "bedroom", "stars", "derive", "Future", "past"};
    //keep track of the randomly generated room indices so you don't reuse them
    //keep track of which rooms have been created
    int used_rooms[10] = {0,0,0,0,0,0,0,0,0,0};
    //create all 7 rooms
    //index variable will be randomized to set which room is going to be used
    int index = 0;
    for (int i = 0; i < ROOM_COUNT; i++)
    {
        //while the room exists at the radom chosen index, keep randonmizing it until new index without a room is found
        while (used_rooms[index] == 1)
        {
            //randomize the index
            index = rand() % 10;
        }
        //set the random index to 1 (create a room at random index 1)   
        used_rooms[index] = 1;
        //copy the name of the randomly chosen room to the ith index in the rooms struct (Creating a random room in the struct)
        strcpy(rooms[i].room_name, room_names[index]);
        //set the number of connections for each created room (struct) to zero
        rooms[i].numconnections = 0;
    }
    //copy "start room" to the first room
    strcpy(rooms[0].room_type, "START_ROOM");
    //copy "end room" to the last room 
    strcpy(rooms[ROOM_COUNT - 1].room_type, "END_ROOM");
    //copy "Mid Room" to all of the middle rooms
    for (int i = 1; i < ROOM_COUNT - 1; i++)
    {
        strcpy(rooms[i].room_type, "MID_ROOM");
    }
}


//Connect_rooms function: In this function I loop through all of the rooms and randomly connect them to eachother
//1. set a target room to connect to
//2. if that connection does not exist, create the connection from the room to the target and vice versa
void connect_rooms()
{
    //iterate through the randomly created rooms
    for (int i = 0; i < ROOM_COUNT; i++)
    {
        //Generate the number of random connections for each room
        int random_num_connections = (rand() % (MAX_CONNECTIONS - MIN_CONNECTIONS + 1)) + MIN_CONNECTIONS;
        //iterate until all of the connections are created
        while (rooms[i].numconnections < random_num_connections)
        {   
            //set a random room (target to connect to from the original room)
            int target = rand() % ROOM_COUNT;
            // check if the target room is not the current room, as well as if the number of connections is less than the room count and that the target connections are less than room count
            if (target != i && rooms[i].numconnections < ROOM_COUNT && rooms[target].numconnections < ROOM_COUNT )
            {
                //check if the connection already exists
                int exists = 0;
                //for each of the connections, if it exists, break out of the loop and stop the connection
                for (int j = 0; j < rooms[i].numconnections; j++)
                {
                    //check if connection already exists and if it does, break out
                    if (strcmp(rooms[i].connections[j], rooms[target].room_name) == 0) 
                    {
                        exists = 1;
                        break;
                    }
                }
                //if the connection does not exist, copy the roomname of the room to the randomized connection target and connect the target to the original room (make sure the room connections are bidirectional)
                if (!exists)
                {
                    //copy the target room name connection to the array of current room connections and increment the number of connections created
                    strcpy(rooms[i].connections[rooms[i].numconnections++], rooms[target].room_name);
                    //copy the room name of the connecting room the the target (connected room), and increment the number of connections
                    strcpy(rooms[target].connections[rooms[target].numconnections++], rooms[i].room_name);
                }
                
            }
        }
    }
}


//write_room_files function: this function writes all rooms' information into the files
//1. Create a file for each room
//2. Write the name of the room in the file
//3. Write the Connections for each room in the file
//4. Write the type of the room in the file
//5. close the file
void write_room_files() {
    //iterate through all of the generated rooms and create a file with the room name
    for (int i = 0; i < ROOM_COUNT; i++) {
        //buffer for the name of the file
        char filename[100];
        //format the name of the created file with the PID and the room name
        snprintf(filename, sizeof(filename), "bdaly.rooms.%d/%s", getpid(), rooms[i].room_name);
        //open the file and make it writeable
        FILE *file = fopen(filename, "w");
        //check if the file is created properly
        if (!file) {
            perror("Error creating room file");
            exit(1);
        }
        //Write the room name to the file
        fprintf(file, "ROOM NAME: %s\n", rooms[i].room_name);
        //write all of the rooms' connections to the file
        for (int j = 0; j < rooms[i].numconnections; j++) {
            fprintf(file, "CONNECTION %d: %s\n", j + 1, rooms[i].connections[j]);
        }
        //write the room type to the file
        fprintf(file, "ROOM TYPE: %s\n", rooms[i].room_type);
        //close the file
        fclose(file);
    }
}

//Start Function: Runs the Game ... Reads the files to Print the info to the User, grabs the stdinput from the user to play the game
//show current room
//show possible locations
//get user input
void start()
{
    //create a buffer for the filename of the file that is going to be read (specified by the user but initialized to the start room in the first run)
    char room[256];
    //format the string for the room the user wants to read
    snprintf(room, sizeof(room), "bdaly.rooms.%d/%s", getpid(), rooms[0].room_name);
    //Set an index to determine which room in the struct we want to switch to (this is how the game switches to the room intended by the user)
    int currentroomindex = 0;
    //Initialize the steps accumulator
    int steps = 0;
    //read the file and save the array with information into a pointer to a pointer (for an array of strings)
    char** temparray = filereader(room);
    //Run the Game until the end room is reached
    while (strcmp(rooms[currentroomindex].room_type, "END_ROOM") != 0)
    {
        //print current location
        printf("CURRENT LOCATION: %s\n", rooms[currentroomindex].room_name);
        //Read the specified room and save the processed data
        char** temparray = filereader(room);
        //Set a flag for knowing when to print a comma in the list of possible connections
        int first = 1;
        //Print the possible connections in a row (grabbed from the processed data from the read file function)
        printf("POSSIBLE CONNECTIONS: ");
        //iterate through the processed data to get all of the connected rooms for printing to the user
        for (int i = 1; i < MAX_TOKENS; i++)
        {
            if (temparray[i] != NULL)
            {
                //remove the \n and change it to \0 to signify end of string (Needed to do this for string comparison)
                //strcspn determines tne amount of number of charaters before the \n then replaces it with \0
                temparray[i][strcspn(temparray[i], "\n")] = '\0';
                //Make sure Start room and Mid Room are not accounted for as connections
                if (strcmp("START_ROOM", temparray[i]) != 0 && strcmp("MID_ROOM", temparray[i]) != 0)
                {
                    //make sure commas are printed after the first connection
                    if (!first)
                    {
                        printf(", ");
                    }
                    printf("%s", temparray[i]);
                    //after the first connection, set the flag to zeros so commas can be printed
                    first = 0;
                }
            }
        }
        //print a . and a newline
        printf(".\n");
        //set a buffer for user input to be saved to
        char user_input[100];
        //Prompt the user to determine which room to go to next
        printf("WHERE TO? >");
        //use fgets to pause and get the standard input from the user for the direction to the next room
        fgets(user_input, sizeof(user_input), stdin);
        //remove the newline and replace it to end of string operator for comparison
        user_input[strcspn(user_input, "\n")] = '\0';
        //print newline
        printf("\n");
        //set a flag to check if the room the user inputted is a connection
        int true = 0;
        //check is user input is one of the connections
        for (int i = 1; i < MAX_TOKENS; i++)
        {
            if (temparray[i] != NULL)
            {
                //replace the newline to an end of string character
                temparray[i][strcspn(temparray[i], "\n")] = '\0';
                //make a copy of temparray
                char* temp = temparray[i];
                //if the user's input is the same as a connection, set the flag true to 1 and break out of loop
                if (strcmp(user_input, temparray[i]) == 0)
                {
                    true = 1;
                    break;
                }
            }
        }
        //if the user did not input a value room connection, print an error message and let them input another answer
        if (true == 0)
        {
            printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
        }
        //if the user specified a real connection, check the room they specified and go to that room
        else
        {
            //go to the room by making the inputted rooms's id the current room idx number
            //check is the user input/connection is also a room that exists and go to that room
            for (int i = 0; i < ROOM_COUNT; i++)
            {
                //if the user input is in the list of rooms, changed the current room index to go to that room
                if (strcmp(user_input, rooms[i].room_name) == 0)
                {
                    currentroomindex = i;
                    break;
                }
            }
            //input the inputted room to the path
            path[path_idx] = rooms[currentroomindex].room_name;
            //update the index in the path array
            path_idx ++;
            //update the number of steps
            steps ++;
            //update the roomname for the next search to grab the right file to search
            snprintf(room, sizeof(room), "bdaly.rooms.%d/%s", getpid(), rooms[currentroomindex].room_name);

        }
    }

    //when the end room has been reached, congratulate the user adn print the number of steps they took
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
    printf("YOU TOOK %i STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
    
    //print all of the steps the user took (their path)
    //print the visted rooms with newlines after each one
    for (int i = 0; i < MAX_PATH; i++)
    {
        if (path[i] != NULL)
        {
            printf("%s\n", path[i]);
        }
    }

    //set all of the array indices to null to avoid memory leaks and free the temporary array since it was malloced in the other function
    for (int i = 0; i < MAX_TOKENS; i++) {
        if (temparray[i] != NULL) {
            free(temparray[i]);
        }
    }
    free(temparray);
}

//filereader function: open the specified room file as a readable file then process the data that is printed into an array
char** filereader(char* roomName)
{
    //open the RoomName file as a readable file
    FILE* file = fopen(roomName, "r");
    //check if file is NULL
    if (file == NULL)
    {
        perror("Cannot open file");
        return NULL;
    }
    
    //allocate space for the array of data we process
    char** arrayoftokens = malloc(MAX_TOKENS * sizeof(char*));
    
    //print an error message if the malloc failed to initialize the array and close the file
    if (!arrayoftokens) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    //create a buffer to read the file
    char buffer[MAX_LENGTH];
    //create an index to add the data to the array
    int token_index = 0;

    //read each of the lines of the file and makesure the token indexes don't pass the max tokens
    while (fgets(buffer, sizeof(buffer), file) && token_index < MAX_TOKENS)
    {
        //copy string into a buffer
        char copy[50];
        strcpy(copy, buffer);
        
        //strtok buffer and remove whitespaces
        // Tokenize using strtok
        // First token (ROOM)
        char* tempstr = strtok(copy, " ");  
        // Second token (NAME)
        char* second_token = strtok(NULL, " "); 
        // Third token (NAME)
        char* third_token = strtok(NULL, " "); 

        //put the third token into the array of tokens
        if (token_index < MAX_TOKENS) {
            //using strdup to dynamically allocate space for new string and copy the third token into that space for the array
            //had to use because I kept getting a segmentation fault with the static initialization
            arrayoftokens[token_index++] = strdup(third_token);
        }
    }

    //close the file when done reading
    fclose(file);
    
    //this sets any unused tokens to NULL to prevent unwanted memory behavior
    for (int i = token_index; i < MAX_TOKENS; i++) {
        arrayoftokens[i] = NULL;
    }
    //return the processed data
    return arrayoftokens;

}
