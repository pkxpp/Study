// DllTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "dll1.h"
#include "../dll2/dll2.h"
#include <windows.h>
//using namespace TestDll1;

#define RTLD_LAZY 0
#define my_dlopen(name,flag) LoadLibrary(name)
#define my_dlsym(handle,fname) GetProcAddress((HMODULE)handle,fname)
#define my_dlclose(handle) FreeLibrary((HMODULE)handle)

//_stdcall
typedef int( * AddProc)(int, int);//定义函数指针类型


int _tmain(int argc, _TCHAR* argv[])
{
	int nRet = sum_test(3, 5);
	cout << "nRet = " << nRet << endl;
	///////////
	HINSTANCE hDBhandle = LoadLibrary(L"dll2.dll");
	if (hDBhandle)
	{
		AddProc pFunc = (AddProc)GetProcAddress(hDBhandle, "sum_test");
		if (!pFunc){
			cout << "获取函数失败!" << endl;
			getchar();
			return 0;
		}
		int nRet1 = pFunc(3, 5);
		cout << "nRet1 = " << nRet1 << endl;
	}
	else
	{
		cout << "Open dll failed!" << endl;
	}
	int nRet3 = sum_test(3, 5);
	cout << "nRet3 = " << nRet << endl;
	///////////////////
	getchar();
	return 0;
}

