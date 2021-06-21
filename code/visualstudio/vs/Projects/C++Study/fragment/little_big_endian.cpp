#include "stdafx.h"

using namespace std;

const int MAX_SIZE = 4;
union CheckEndian{
	int nInter;
	char cArrays[MAX_SIZE];
};

// 参考网上的方法
static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.mylong)

void check_endian(){
	CheckEndian t;
	t.nInter = 1;
	cout << "check the system endian ..." << endl;
	cout << "cArrays[0] = " << t.cArrays[0] << endl;
	bool bIsLittle = t.cArrays[0] == 1;
	if (bIsLittle)
		cout << "The system is: Little-Endian." << endl;
	else
		cout << "The system is: Big-Endian." << endl;

	cout << ENDIANNESS << endl;
}