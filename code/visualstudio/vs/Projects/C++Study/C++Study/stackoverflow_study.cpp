//////////////////////////////////////////////////////////////////////////
// file		: stackoverflow.cpp
// author	: pk
// time		: 2019/06/03
// desc		: some content can't class puted here
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "stackoverflow_study.h"

using namespace std;

// https://stackoverflow.com/questions/56420390/converting-a-pointer-to-x-to-an-array-of-x
void convert_point_to_array()
{
	auto fnTestGetSize = [](int nType)->size_t
	{
		switch (nType)
		{
		case 0:
			return 50;
		default:
			return 100;	
		}
	};
	size_t array_size = fnTestGetSize(0);
	char *foo = new char[array_size];
	if (array_size > 42)
		foo[42] = 'X';

	cout << foo[42] << endl;
}
/*summary
 * in vs2015 it's ok.And the question is happened in MISRA C++
 */
