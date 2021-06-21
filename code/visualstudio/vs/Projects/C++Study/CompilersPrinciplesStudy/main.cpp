#include <iostream>
#include <atomic>
#include "StaticLib.h"

class TestClass {
public:
	std::atomic<void*> Ptr;
	int Value;
};

int fun(int a, int b);
int m = 10;
int main()
{
	//int i = 4;
	//int j = 5;
	//m = fun(i, j);

	//TestClass test;
	//std::cout << test.Ptr << std::endl;

	// link error
	//TestUseStrcutInOtherStaticLib();
	StalicLib a;
	//a.UseStrcutInOtherStaticLib2(Vec2List);
	a.UseStrcutInOtherStaticLib3();

	getchar();
	return 0;
}

int fun(int a, int b)
{
	int c = 0;
	c = a + b;
	return c;
}