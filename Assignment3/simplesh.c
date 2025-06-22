#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

//define constant parameters
#define MAX_COMMAND_LENGTH 1028
#define MAX_ARGUMENT_LENGTH 512

//initialize functions
void execute_command(char* input_file, char* output_file, char** arguments, int backgroundflag, int argumentsptr, pid_t background_pids[], int *bg_count);
void cd(int argumentsptr, char** arguments);
void execute_foreground_process(char** arguments, char* input_file, char* output_file);
void execute_background_process(char** arguments, char* input_file, char* output_file, pid_t background_pids[], int *bg_count);
void status(int status);
void check_background_processes(pid_t background_pids[], int *bg_count);


//set int for the last status number to check on the status of the last foreground process
int last_foreground_status = 0;

int main() 
{
    //ignore the CTRL C signal in the shell
    signal(SIGINT, SIG_IGN); 
    //create an array of backround pids to check on background processes
    pid_t background_pids[MAX_ARGUMENT_LENGTH];
    //create an integer to check the amount of background processes
    int bg_count = 0;
    //create an array to hold the user input
    char user_input[MAX_COMMAND_LENGTH];
    //create an array to hold the input file name
    char* input_file = NULL;
    //create an array to hold the output file name
    char* output_file = NULL;
    //create an integer to hold the flag for a background process
    int backgroundflag = 0;
    //create an array to hold all of the arguments inputted by the user
    char *arguments[MAX_ARGUMENT_LENGTH];
    //make a tracker for the arguments in the array of arguments
    int argumentsptr = 0;

    //run shell until exit typed into it
    while (1)
    {
        //check the background processes
        check_background_processes(background_pids, &bg_count);
        //make the cursor a :
        printf(": ");
        //flush the standard output
        fflush(stdout);
        //get the user input from the user
        fgets(user_input, MAX_COMMAND_LENGTH, stdin);

        //check if there is a comment or newline and pass if so
        if (user_input[0] == '#' || user_input[0] == '\n')
        {
            continue;
        }

        // Split the user input spaces and newline
        char* token = strtok(user_input, " \n");

        //iterate through the tokens of the splitted string
        while (token != NULL)
        {
            //check to see if & used to signify background process, if so, set the flag to 1 for later
            if(strcmp(token, "&") == 0)
            {
                backgroundflag = 1;
            }
            //check if input file redirection is used by the user, if so, store the input file
            else if(strcmp(token, "<") == 0)
            {
                input_file = strtok(NULL, " \n");
            }
            //check if output redirection used by user, if so, store the output file name
            else if(strcmp(token, ">") == 0)
            {
                output_file = strtok(NULL, " \n");
            }
            //store all of the arguments into an array and keep track of where in array lasta argument is
            else
            {
                arguments[argumentsptr] = token;
                argumentsptr++;
            }
            // go to the next token
            token = strtok(NULL, " \n");
        }
        //put a NULL into the array after the last argument
        arguments[argumentsptr] = NULL;  

        //exit command for exiting the terminal
        if (arguments[0] != NULL && strcmp(arguments[0], "exit") == 0)
        {
            exit(0);
        }
        //cd command to change to user inputted directories
        else if (arguments[0] != NULL && strcmp(arguments[0], "cd") == 0)
        {
            cd(argumentsptr, arguments);
        }
        //status command to check status of last foreground process
        else if (arguments[0] != NULL && strcmp(arguments[0], "status") == 0)
        {
            status(last_foreground_status);
        }
        //if none of previous commands inputted, execute any other existing command the user inputs
        else
        {
            execute_command(input_file, output_file, arguments, backgroundflag, argumentsptr, background_pids, &bg_count);
        }

        //reset the background flag, arguments pointer, and make the input and output files Null for next command inputs
        backgroundflag = 0;
        argumentsptr = 0;
        input_file = NULL;
        output_file = NULL;
    }
}

//change directory fucntion
void cd(int argumentsptr, char** arguments)
{
    //set teh home directory in the home variable using getenv
    char* home = getenv("HOME");

    //go to the home directory if only cd inputted by user
    if (argumentsptr == 1)
    {   
        chdir(home);
    }   
    //if the user inputs cd then an argument
    else if (argumentsptr == 2)
    {
        //remove teh newline at the end of the directory user wants to change to
        arguments[1][strcspn(arguments[1], "\n")] = 0; 
        //if the user inputs a ~ after the cd, go to home directory
        if (strcmp(arguments[1], "~") == 0)
        {
            chdir(home);
        }
        //otherwise, go to whatever existing directory the user inputs
        else
        {
            chdir(arguments[1]);
        }
    }
}

//execute the foreground and background commands inputted by the user
void execute_command(char* input_file, char* output_file, char** arguments, int backgroundflag, int argumentsptr, pid_t background_pids[], int *bg_count)
{
    //check if the background flag is a 1 or 0 to determine whether to use background of foreground process
    if (backgroundflag == 1)
    {
        execute_background_process(arguments, input_file, output_file, background_pids, bg_count);
    }
    else
    {
        execute_foreground_process(arguments, input_file, output_file);
    }
    
}

//this function executes the background processes
void execute_background_process(char** arguments, char* input_file, char* output_file, pid_t background_pids[], int *bg_count)
{
    //create a child process for the background
    pid_t pid = fork();

    //if the pid is less than zero, there is an error with the fork
    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    //if the pid is 0, this is the child process, and the parent should not wait for the child process since it is in the backgorund
    if (pid == 0)
    {
        // If the input file redirection is stated by the user, open the input file and redirect the contents to stdin
        if (input_file != NULL)
        {
            //open the input file
            int filedescriptor = open(input_file, O_RDONLY);
            //if the file does not exist, give an error
            if (filedescriptor < 0)
            {
                perror("Error opening input file");
                exit(1);
            }
            //duplicate the file contents to standardinput
            dup2(filedescriptor, 0);
            //close the file
            close(filedescriptor);
        }
        else
        {
            // Redirect input to /dev/null for background processes
            int filedescriptor = open("/dev/null", O_RDONLY);
            // give an error if the file does not exist
            if (filedescriptor < 0)
            {
                perror("Error opening /dev/null");
                exit(1);
            }
            //redirect /dev/null to standard input
            dup2(filedescriptor, 0);
            //close the file
            close(filedescriptor);
        }

        // Redirect to output file if the user specifies this argument
        if (output_file != NULL)
        {
            //open the output file and make it writeable and able to append to
            int filedescriptor = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
            //if there is no output file that exists, throw an error
            if (filedescriptor < 0)
            {
                perror("Error opening output file");
                exit(1);
            }
            //redirect contents to standard output
            dup2(filedescriptor, 1);
            //close outputfile 
            close(filedescriptor);
        }

        // Execute command specified by user
        execvp(arguments[0], arguments);
        //if the command does not exist, give an error to user and exit with 1
        perror("Command not found");
        exit(1);
    }
    //parent (should not wait for child pid to finish process since it is in background)
    else
    {
        //print the background pid and flush the standard output
        printf("Background PID: %d\n", pid);
        fflush(stdout);

        // Store background PID in tracking array
        background_pids[*bg_count] = pid;
        //increment the amount of background processes
        (*bg_count)++;
    }
}

//this function manages the foreground processes
void execute_foreground_process(char** arguments, char* input_file, char* output_file)
{
    //create a child process with fork (create process to run execution)
    //to run in background, no need for parent to wait for child process to execute to finish
    pid_t pid = fork();
    //give an error if the pid is less than 0
    if (pid < 0)
    {
        perror("Fork Failed to create!");
        exit(1);
        
    }
    //this is the child process, must run while parend waits
    if (pid == 0)
    {
        //if user throws CTRL C signal during child process running, exit the process
        signal(SIGINT, SIG_DFL); // Restore SIGINT behavior for the child
        // If the input file redirection is stated by the user, open the input file and redirect the contents to stdin
        if (input_file != NULL)
        {
            //open the input file
            int filedescriptor = open(input_file, O_RDONLY);
            
            //if the filedescripter is less than zero, give an error stating input file doesn't exist
            if ( filedescriptor < 0 )
            {
                printf("Error with opening input file.\n");
                exit(1);
            }
            
            //redirect to standard input
            dup2(filedescriptor, 0);
            //close the file
            close(filedescriptor);
        }
        //if the output file exists as specified by user
        if (output_file != NULL)
        {
            //open the output file
            int filedescriptor = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
            //if the file descriptor is less than zero, give an error saying output file does not exist
            if ( filedescriptor < 0 )
            {
                printf("Error with opening output file.\n");
                exit(1);
            }
            //redirect the file contents to stdoutput
            dup2(filedescriptor, 1);
            //close the file
            close(filedescriptor);
        }
        //execute the user-specified command
        execvp(arguments[0], arguments);
        //throw an error if the command does not exist
        perror("Command doesn't exist");
        exit(1);
        
    }
    //this is the parent process
    else
    {
        //check the status of the child pid and when the child is finished running, run the parent process
        int status;
        waitpid(pid, &status, 0);

        //if the process exited normally, make the last process status the tracked status
        if (WIFEXITED(status)) 
        {
            last_foreground_status = status;
        }
        //if the process is terminated by a signal
        else if (WIFSIGNALED(status)) 
        {
            //capture the signal that terminated the status
            int term_signal = WTERMSIG(status);
            //display which signal terminated the process
            printf("Terminated by signal %d\n", term_signal);
            //flush standard output
            fflush(stdout);
            //set the last  process status to 1
            last_foreground_status = status;
        }
    }
}


//this function tracks the status of the last foreground process
void status(int status)
{
    //set exit and signal values
    int exit = WEXITSTATUS(status);
    int signal = WTERMSIG(status);
    //if the process exits, print the exit value
    if (WIFEXITED(status))
    {
        printf("exit value %i\n", exit);
    }
    //if a signal interrupts the process, print the signal that does
    else if(WIFSIGNALED(status))
    {
        printf("Terminated by signal %d\n", signal);
    }

}


//this function checks the background processes
void check_background_processes(pid_t background_pids[], int *bg_count)
{
    int status;
    //iterate through the background processes
    for (int i = 0; i < *bg_count; i++) {
        // Check the status of a background process without blocking (WNOHANG)
        pid_t pid = waitpid(background_pids[i], &status, WNOHANG);

        // If the process has terminated (pid > 0)
        if (pid > 0) {
            // Print process id and termination detail
            printf("Background process %d terminated", pid);
            //print exit status if the process exited normally
            if (WIFEXITED(status)) 
            {
                printf(" with exit status %d\n", WEXITSTATUS(status));
            } 
            //if the signal interrupted the process, state teh signal
            else if (WIFSIGNALED(status))
            {
                printf("due to signal %d\n", WTERMSIG(status));
            }
            //flush the output
            fflush(stdout);

            // Remove process from tracking array
            for (int j = i; j < (*bg_count) - 1; j++) {
                //shift the array left to fill in the gap
                background_pids[j] = background_pids[j + 1];
            }
            //decrement the count of bacground processes
            (*bg_count)--;
            //adjust index to make sure next process is accounted for
            i--; 
        }
    }
}
    