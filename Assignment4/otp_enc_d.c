
#include <arpa/inet.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

//define max message lengths and max socket requests and amount of letters + space
#define MAX_MESSAGE_LENGTH 25000
#define MAX_SOCKET_REQUESTS 5
#define ALPHABET_SIZE 27  
#define MAXLEN 1000

//initialze functions
void opt_enc_d(int port);
int* map_letter_to_number(char* plaintext_sentence, int* size);
char* encrypt(char* buff);

//get the port and run otp_dec
int main(int argc, char* argv[])
{
   int port = atoi(argv[1]);
   opt_enc_d(port);
   return 0;
}


//Use this function to send and receive data from accepted sockets
void handle_client(int client_fd, struct sockaddr_in peerinfo) {
    //set buffer and peer address and message length
    char peeraddr[INET_ADDRSTRLEN], buff[MAXLEN+1];
    int mlen;

    inet_ntop(AF_INET, &peerinfo.sin_addr.s_addr, peeraddr, sizeof(peeraddr));

    // Receive data from client
    mlen = recv(client_fd, buff, sizeof(buff) - 1, 0);
    if (mlen > 0) {
        buff[mlen] = '\0'; // Null-terminate received string
    }

    //encrypt the message
    char* encryptedmessage = encrypt(buff);

    // Send response
    send(client_fd, encryptedmessage, strlen(encryptedmessage), 0);

    // Close the client socket and exit child process
    close(client_fd);
    exit(0);
}
    

//otp_enc_d
//will run in background as a daemon
//perform the encoding (message _ key mod 27)
//listen on a particular port assigned when it is first run
//receives plaintext and a key via the port when a connection to it is made
//write back the ciphertext to the process that it is connected to by the same port
//key must be as big as the plaintext
//otp enc d makes a connection with otp enc -> it will fork off a process immediately and available to receive more connections
//must support up to 5 concurrent socket connections
//encryption will take place in the process and ciphertext will be written back
//can either create new process every time a connection is made or set up a pool of 5 processes to handle encryption tasks
void opt_enc_d(int port)
{
    //initialize variables
    int lengthofmessage = 0;
    int server_fd = 0;
    int client_fd = 0;
    struct sockaddr_in server_address, client_fd_info;
    socklen_t len;

    char buff[MAX_MESSAGE_LENGTH+1];  
    char message[MAX_MESSAGE_LENGTH], key[MAX_MESSAGE_LENGTH], encoded_message[MAX_MESSAGE_LENGTH];
    int sum[MAXLEN];    
    
    //ignore SIGCHLD signals since parent is not tracking the child signals (running in background)
    signal(SIGCHLD, SIG_IGN);


    //create socket and check if it fails
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket Error OTP_ENC_D");
    }

    //specifies that the server is using UNIX address
    server_address.sin_family = AF_INET;
    //converts the port to network byte order
    server_address.sin_port = htons(port);
    //listen on any interface
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    
    //bind the socket to the port and check if it fails
    if (bind(server_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        close(server_fd);
    }

    //start listening for incoming connection requests 
    if (listen(server_fd, MAX_SOCKET_REQUESTS) < 0)
    {
        perror("Listen Error");
        close(server_fd);
    }

     // Main server loop
     while (1) {
        // Accept new connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_fd_info, &len);
        if (client_fd < 0) {
            perror("Accept failed");
            exit(1);
        }

        // Fork a new process to handle the client
        pid_t pid = fork();
        if (pid == 0) 
        { 
            // Child process
            close(server_fd); // Child doesn't need the listening socket
            handle_client(client_fd, client_fd_info);
        } 
        else if (pid > 0) 
        {
             // Parent process
             // Parent doesn't need the connected socket
            close(client_fd); 
        } 
        else 
        {
            perror("Fork failed");
            close(client_fd);
        }
    }

    //shutdown server and client
    shutdown(server_fd, SHUT_RDWR);
    shutdown(client_fd, SHUT_RDWR);
}


//iterate through string (array of characters)
//map letters to characters
int* map_letter_to_number(char* plaintext_sentence, int* size)
{
    //get length of plaintext string
    int length = strlen(plaintext_sentence);
    //allocate space for temporary array
    int* temp_array = (int*) malloc(length * sizeof(int));
    int count = 0;

    if (temp_array == NULL)
    {
        perror("failed malloc");
    }

    // Iterate through string and map letters to numbers
    for (int i = 0; i < length; i++)
    {
        //check if it is a letter
        if (isalpha(plaintext_sentence[i]))  
        {
            //make sure it is an uppercase letter
            char uppercase = toupper(plaintext_sentence[i]); 
            //convert to number 
            temp_array[count++] = uppercase - 'A';  
        }
        else if (plaintext_sentence[i] == ' ')
        {
            temp_array[count++] = 26;
        }
    }

    *size = count;  // Update the size of the array


    int* numerical_map_array = (int*) realloc(temp_array, count * sizeof(int));
    if (numerical_map_array == NULL) 
    {
        perror("failed realloc");
        return temp_array;  // Return the original allocation
    }


    return numerical_map_array;
}




char* encrypt(char* buff)
{
    char message[MAX_MESSAGE_LENGTH], key[MAX_MESSAGE_LENGTH], encoded_message[MAX_MESSAGE_LENGTH];
    char* encoded_message_final;
    int sum[MAXLEN];    
    int size_message = 0;
    int size_key = 0;
    int sumptr = 0;
    int charptr = 0;
    char encoded_char;

    //tokenize buff before mapping  letter to number
    //map both key and plaintext to numbers
    //add and do modulus operation
    char* buffertoken = strtok(buff, ">");
    if (buffertoken != NULL)
    {
        strcpy(message, buffertoken);
        buffertoken = strtok(NULL, ">");

    }
    int* mapped_message = map_letter_to_number(message, &size_message);
    if (buffertoken != NULL)
    {
        strcpy(key, buffertoken);
        //printf("\n key: %s\n",  buffertoken);
        buffertoken = strtok(NULL, ">");

    }                
    int* mapped_key = map_letter_to_number(key, &size_key);

    if (size_key >= size_message )
    {
        for (int i = 0; i < size_message; i++)
        {
            
            int sumint = mapped_key[i] + mapped_message[i];
            int mod = sumint % 27;
            sum[sumptr] = mod;
            sumptr ++;
            //need to reset sumptr
        }
        
        for (int j = 0; j < sumptr; j++)
        {
            if (sum[j] == 26)
            {
                encoded_char = ' ';
            }
            else
            {
                encoded_char = (char)(sum[j] + 'A');
            }
            encoded_message[charptr] = encoded_char;
            charptr ++;
        }
        free(mapped_key);
        free(mapped_message);
        encoded_message[charptr] = '\n';
        encoded_message[charptr + 1] = '\0';

        
        
        
        
    }
    else
    {
        printf("Key length less than message length");
        exit(1);
    }

    
    sumptr = 0;
    charptr = 0;

    encoded_message_final = encoded_message;
    return encoded_message_final;
}
    
