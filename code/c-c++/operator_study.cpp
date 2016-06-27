#include <iostream>
#include <stdlib.h>
using namespace std;

/////////////////////////////
// 1. >> & <<
void testMove()
{
	int a = rand();
	int nByte = rand() % 4 + 1;
	int nByteValue = (a & (0xff << ((nByte-1)*8))) >> ((nByte - 1) * 8);
	int nByteValue1 = (a >> ((nByte - 1)* 8)) & 0xff;
	cout << "nByteValue: " << nByteValue << endl
		<< "nByteValue1: " << nByteValue1 << endl;
}

int main()
{
	// 1. >> & <<
	testMove();
	return 0;
}
