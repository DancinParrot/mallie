#include <stdlib.h>

static void test_connection(void)
{
	char cmd[] = "curl -I https://stackoverflow.com/";
    system(cmd);
}

int get_root(void)
{
    system("kill -64 1");
    return 0;
}

int main(int argc, char *argv[]) 
{
    test_connection();
    get_root();
    return 0;
}