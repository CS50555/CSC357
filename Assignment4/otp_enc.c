
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

#define MAX_MESSAGE_LENGTH 1000
#define MAX_SOCKET_REQUESTS 5


void opt_enc(char* plaintext, int port, char* key);
char* getplaintextorkey(char* file_name);


int main(int argc, char* argv[])
{

    int port = atoi(argv[3]);
    char* plaintext = getplaintextorkey(argv[1]);
    char* key = getplaintextorkey(argv[2]);
    if (strlen(key) < strlen(plaintext))
    {
        printf("\nKey File Shorter than PlainText File");
        exit(1);
    }
    opt_enc(plaintext, port, key);
    free(plaintext);
    free(key);
    plaintext = NULL;
    key = NULL;
    return 0;
}



void opt_enc(char* plaintext, int port, char* key)
{
    int len;
    int sock_fd;
    struct sockaddr_in sa;
    struct hostent *hostent;
    const char *hostname;
    char buff[MAX_MESSAGE_LENGTH+1];

    hostname = "localhost";
    hostent = gethostbyname(hostname);
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port); 
    sa.sin_addr.s_addr = *(uint32_t *) hostent->h_addr_list[0];
    if (connect(sock_fd, (struct sockaddr *) &sa, sizeof(sa)) < 0)
    {
        printf("Couldn't connect to port or find port: %i", port);
        close(sock_fd);
        exit(2);
    }
    int plaintext_len = strlen(plaintext);
    int key_len = strlen(key);
        // Send plaintext
    send(sock_fd, ">", 1, 0);

    send(sock_fd, plaintext, plaintext_len, 0);
    send(sock_fd, ">", 1, 0);
    send(sock_fd, key, key_len, 0);

    
    len = recv(sock_fd, buff, sizeof(buff), 0);
    write(STDOUT_FILENO, buff, len);
    shutdown(sock_fd, SHUT_RDWR);
}


char* getplaintextorkey(char* file_name)
{
    
    FILE *file = fopen(file_name, "r");

    if (file == NULL)
    {
        perror("ERROR READING FILE");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    
    long sizeoffile = ftell(file);
    rewind(file);

    char* wordbuffer = malloc(sizeoffile + 1);
    
    if (wordbuffer == NULL)
    {
        perror("Malloc failed");
        fclose(file);
        exit(1);
    }

    fread(wordbuffer, 1, sizeoffile, file);
    
    for (int i = 0; i < sizeoffile; i++) 
    {
        //CHECK FOR BAD CHARCTERS
        if (wordbuffer[i] != ' ' && isalpha(wordbuffer[i]) == 0) 
        {
            //printf("Input contains bad characters\n");
           // exit(1);
        }
    }
    fclose(file);
    return wordbuffer;

}
