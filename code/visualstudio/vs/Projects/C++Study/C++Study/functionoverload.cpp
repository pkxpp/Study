//////////////////////////////////////////////////////////////////////////
// 函数重载
// 2018/11/09
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <stdlib.h>
using namespace std;
//////////////////////////////////////////////////////////////////////////
// 1.重载函数匹配规则
// 问题：c#调用c++ CLI接口，UInt64没有匹配到__int64，而是匹配到了float，真是惊呆了

void pritnfA(int a)
{
	printf("prinfA(int a)\n");
}

void pritnfA(__int64 a)
{
	printf("prinfA(__int64 a)\n");
}

void pritnfA(float a)
{
	printf("prinfA(float a)\n");
}

void TestOverloadMatchRule()
{
	__int64 a = 5;					// __int64
	//unsigned __int64 a = 5;		// error: ambious
	long b = 6;						// int
	unsigned int c = 7;
	//pritnfA(c);					// error C2668: 'pritnfA' : ambiguous call to overloaded function
}