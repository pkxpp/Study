//////////////////////////////////////////////////////////////////////////
// 
// c++ study
// pk
// 2013/05/24
// 
//////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <iostream>
#include "StringStudy.h"

using namespace std;

/************************************************************************/
/* sizeof(unsigned long long)  
 * 2013/05/24
 * nRet = g_GlobalVariable.m_sPostOffice.RemoveInBoxMail(string(pNpc->m_szName), *(unsigned long long*)uParam);
 * uParam∂®“Â unsigned int uParam
 */
/************************************************************************/
int test_1()
{
	printf("%d, %d", sizeof(unsigned int), sizeof(unsigned long long));

	return 0;
}

/************************************************************************/
/* printf("%d, %d", n++, n++)
 * 2014/04/10
 */
/************************************************************************/
int test_2()
{
	int n = 1;
	printf("%d, %d", n++, n++);

	return 0;
}

/************************************************************************/
/* 3. check big-little endian
 * 2014/10/14
 */
/************************************************************************/
int CheckEndian()
{
	{
		union w
		{
			int a;
			char b;
		}c;
		c.a = 1;
		return (c.b == 1);
	}
}

// 3.2 big-endian or small-endian
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

		printf("address: %x, %x, %x\n", &(u.s), &(u.c[0]), &(u.c[1]));
		if (u.c[0] == 1 && u.c[1] == 2)
		{
			cout << "big endian. " << endl;
		}
		else if(u.c[0] == 2 && u.c[1] == 1)
			cout << "little endian." << endl;
	}
}
/************************************************************************/
/* 4. check CPU bit
 * 2014/10/14
 */
/************************************************************************/
void check_system_bit()
{
	cout << sizeof(long) << endl;
	int a = 0;
	printf("%x\n", &a);

	//others
	int b = 0x80000000;
	printf("os is %d\t%d\n", b, sizeof(int));

}

/////////////////////////////////////////////////////////////////////
int main()
{
	//1
	//test_1();

	//2
	//test_2();

	// 3. endian
	if (CheckEndian())
		cout << "big endian" << endl;
	else
		cout << "little endian " << endl;
	// 3.2
	//judge_endian();

	//4. system bit
	//check_system_bit();

	//5. string study
	StringStudy();

	getchar();
	return 0;
}