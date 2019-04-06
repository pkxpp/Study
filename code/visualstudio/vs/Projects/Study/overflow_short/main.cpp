#include <stdio.h>
#include <string.h>


int main()
{
	/*
	short sNum = 0;
	scanf("%d", &sNum);
	printf("sNum = %u", (unsigned short)sNum);

	int nTemp = (unsigned short)sNum;

	printf("nTemp = %u", nTemp);

	int a = 6;
	int b = 10;
	int c = a + b;
	*/

	/************************************************************************/
	/* compare "||" with Lua "or"                                                                     */
	/************************************************************************/
	int a = 2;
	int b = 3;
	int c = a || b;
	printf("c = %d\n", c);
	getchar();
	return 0;
}