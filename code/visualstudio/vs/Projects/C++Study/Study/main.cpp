#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <Windows.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// 目录
// 1.精度丢失问题
// 2.文件读取自动转义问题
//////////////////////////////////////////////////////////////////////////
// 1.精度丢失问题
void test64bit_int2float(){
	int i = 145031173;
	float f = i;
	double fx = i;
	int j = i + float(0);

	// 主要原因还是在计算的时候丢失精度了
	int nCommonAttack = 1;
	int nSkillLevel = 2;
	int k = int(nCommonAttack * 0 / 100
		+ float(1 + nSkillLevel * float(0) / 100)
		* (0 * 0 / 100
		+  0 * 0 / 100
		+  0 * 0 / 100)
		+  0 * 0 / 100
		+  0 * 0 / 100
		+ 0 * 0 / 100
		+ i);

	printf("int(%d) --> f(%f), f1(%f) ---> j = %d\n", i, f, fx, j);
	printf("k = %d\n", k);
	printf("直接转化没问题：%d\n", int(145031173.0));
	printf("运算丢失精度有问题：%d\n", int(145031173 + float(0)));
	printf("%f\n", float(145031173));

	// calculate, implicit convert
	int nTest = 145031173;
	int n1 = nTest + float(0);
	// convert directly
	float f1 = float(nTest);
	
	//printf("n1 = %d\n", n1);
	//printf("f1 = %f\n", f1);

}

//////////////////////////////////////////////////////////////////////////
// 2.文件读取自动转义问题
void ReadFile(char *str){
	HANDLE pFile;
	//pFile = CreateFileW(TEXT(str), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE, NULL);
	if (pFile == INVALID_HANDLE_VALUE)
	{
		int e = GetLastError();
		return;
	}
	DWORD nSize = GetFileSize(pFile, NULL);
	char* pBuff = new char[nSize + 1];
	DWORD nReadSize;
	ReadFile(pFile, pBuff, nSize, &nReadSize, NULL);

	delete [] pBuff;
}

//////////////////////////////////////////////////////////////////////////
int main()
{
	// 1.
	//test64bit_int2float();

	// 2.
	//ReadFile("testio.txt");

	getchar();
	return 0;
}