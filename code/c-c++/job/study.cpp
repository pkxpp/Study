/*
 * pk
 * 2014/10/13
 * some function to study 
 */
#include <iostream>

using namespace std;
///////////////////////////////////////////////////////
// 1. system
// 1.1 judge 32 or 64
void judge_system_bit()
{
	cout << sizeof(long) << endl;
	int a = 0;
	printf("%x\n", &a);
	
	//others
	int b = 0x80000000;
	printf("os is %d\t%d\n", b, sizeof(int));

}
// 1.2 big-endian or small-endian
void judge_endian()
{
	int a = 1;
	char *p = (char*)&a;
	printf("%d, %d, %d, %d, %d\n", (char)a, *p, *(p+1), *(p+2), *(p+3));
	printf("%x, %x\n",  (char*)(&a), &a );
	if((char)a)
		cout << "big endian" << endl;
	else
		cout << "small endian" << endl;
	
	// others
	union ut
	{
		short s;
		char c[2];
	}u;
	if(sizeof(short) == 2)
	{
		u.s = 0x0102;
		if (u.c[0] == 1 && u.c[1] == 2)
		{
			cout << "big endian. " << endl;
		}
		else if(u.c[0] == 2 && u.c[1] == 1)
			cout << "little endian." << endl;
	}
}
















///////////////////////////////////////////////////////
//main
int main()
{
	judge_system_bit();
	judge_endian();
	return 0;
}
