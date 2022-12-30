#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    test_connection();
    return 0;
}

static void test_connection(void)
{
	char cmd[] = "curl -I https://stackoverflow.com/";
    system(cmd);
}