#include <iostream>
using namespace std;

//namespace TestDll1{
	extern "C" _declspec(dllexport) int sum_test(int a, int b);
//};