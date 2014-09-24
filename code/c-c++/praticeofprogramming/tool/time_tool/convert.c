#include <stdio.h>
#include <time.h>


int main(int args, char **argv)
{
	//
	int lTime;
	lTime = atoi(argv[1]);
	printf("lTime:%d\n", lTime);
	time_t time = lTime;
	printf("Local time:%s\n", ctime(&lTime));
	return 0;
}
