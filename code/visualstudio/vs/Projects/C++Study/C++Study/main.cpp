//////////////////////////////////////////////////////////////////////////
// 
// c++ study
// pk
// 2013/05/24
// 
//////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <iostream>
#include <bitset>
#include "StringStudy.h"
#include "lamada_study.h"
#include "stl_study.h"
#include "std_is_same.h"
#include "stackoverflow_study.h"
#include "rvalue_refrences_study.h"
#include "cmd_study.h"

using namespace std;

/************************************************************************/
/* sizeof(unsigned long long)
 * 2013/05/24
 * nRet = g_GlobalVariable.m_sPostOffice.RemoveInBoxMail(string(pNpc->m_szName), *(unsigned long long*)uParam);
 * uParam定义 unsigned int uParam
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
	printf("%d, %d, %d, %d, %d\n", (char)a, *p, *(p + 1), *(p + 2), *(p + 3));
	printf("%x, %x\n", (char*)(&a), &a);
	if ((char)a)
		cout << "big endian" << endl;
	else
		cout << "small endian" << endl;

	// others
	union ut
	{
		short s;
		char c[2];
	}u;
	if (sizeof(short) == 2)
	{
		u.s = 0x0102;

		printf("address: %x, %x, %x\n", &(u.s), &(u.c[0]), &(u.c[1]));
		if (u.c[0] == 1 && u.c[1] == 2)
		{
			cout << "big endian. " << endl;
		}
		else if (u.c[0] == 2 && u.c[1] == 1)
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

/************************************************************************/
/* 6. function overload match rule
 * 2018/11/09
 */
 /************************************************************************/
void TestOverloadMatchRule();
/************************************************************************/
/* 7. c++11
 * 2018/11/15
 *
 * 7.1 lambda
 * 7.2 rvalue references
 */
 /************************************************************************/
 //void TestLambda();
 //void TestRvalueReferences();

 /************************************************************************/
 /* 8. stl test
 * 2018/12/26
 */
 /************************************************************************/
 // testSTLStudy

 /************************************************************************/
 /* 9. std is same compile error
 * 2019/01/24
 */
 /************************************************************************/
 // StdIsSameTest

 /************************************************************************/
 /* 10. stack over flow can't class
 * 2019/06/03
 */
 /************************************************************************/
 // convert_point_to_array
void convert_point_to_array();

/************************************************************************/
/* 11. float test
* 2019/11/07
*/
/************************************************************************/
void float_test()
{
	//float x = -4.10000086;

	float y = 4.76143503;
	float y1 = -0.200000167;
	float y2 = -0.200000077;

	float y1Add = y + y1;
	float y2Add = y + y2;

	std::cout << std::bitset<32>(*(_ULonglong*)&y) << std::endl;
	std::cout << std::bitset<32>(*(_ULonglong*)&y1) << std::endl;
	std::cout << std::bitset<32>(*(_ULonglong*)&y1Add) << std::endl;

	std::cout << std::bitset<32>(*(_ULonglong*)&y2) << std::endl;
	std::cout << std::bitset<32>(*(_ULonglong*)&y2Add) << std::endl;

	cout << "fTest = " << y1Add << endl;
	cout << "fTest1 = " << y2Add << endl;

	// float精度，十进制有效位
	float f = 0.123456789;
	std::cout << std::bitset<32>(*(_ULonglong*)&f) << std::endl;
	cout << f << endl;
	printf("f = %lf\n", f);

	f = 12345.6789f;
	cout << f << endl;
	double df = f;
	cout << df << endl;
	printf("f = %lf\n", f);
	printf("df = %lf\n", df);

	//////////////////////////////////////////////////////////////////////////
	// 以下是浮点数转二进制的学习
	//IEEE754 参考：https://blog.csdn.net/crjmail/article/details/79723051
	{
		//float n = 125.125; // 01000010111110100100000000000000 
		//std::cout << std::bitset<32>(*(_ULonglong*)&n) << std::endl;

		//n = 0.125; // 00111110000000000000000000000000 
		//std::cout << std::bitset<32>(*(_ULonglong*)&n) << std::endl;

		//n = 5.5; // 01000000101100000000000000000000
		//std::cout << std::bitset<32>(*(_ULonglong*)&n) << std::endl;
	}
	

	// 这个也是对的
	{
		//float f = 5.5f;
		//std::bitset<sizeof(float)*CHAR_BIT> foo(*reinterpret_cast<unsigned long*>(&f));
		//cout << foo << endl;
	}
	

	// 这个是错的
	//float f = 0.125;
	//char* bits = reinterpret_cast<char*>(&f);
	//for (std::size_t n = 0; n < sizeof f; ++n)
	//	std::cout << std::bitset<8>(bits[n]);
	//std::cout << '\n';
	// 00000000000000000000000000111110

	// 这个是对的
	{
		//union
		//{
		//	float input;   // assumes sizeof(float) == sizeof(int)
		//	int   output;
		//}    data;

		//data.input = 0.125/*2.25125*/;

		//std::bitset<sizeof(float) * CHAR_BIT>   bits(data.output);


		//std::cout << bits << std::endl;

		//// or

		//std::cout << "BIT 4: " << bits[4] << std::endl;
		//std::cout << "BIT 7: " << bits[7] << std::endl;
	}
	
	//////////////////////////////////////////////////////////////////////////
}
/************************************************************************/
/* 12. c++ definition
* 2019/11/19
*/
/************************************************************************/
/* 13. cmd dos
* 2019/11/20
*/
/************************************************************************/

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
	//StringStudy();

	// 6. function overload match rule
	//TestOverloadMatchRule();

	// 7.c++11

	// 7.1 lambda
	/*TestLambda();*/
	//LambdaStudy lambda;
	//lambda.TestValueAndReferenceCapture();
	//lambda.TestValueAndReferenceCapture(5);

	// 7.2 rvalue references
	//TestRvalueReferences();

	// 8. stl
	//testSTLStudy();

	// 9. std::is_same
	//StdIsSameTest();

	// 10.
	// 10.1
	//convert_point_to_array();

	// 11.
	//float_test();

	// 12.
	Access<double> a;

	// 13.
	CreateConsoleBackground();

	getchar();
	return 0;
}