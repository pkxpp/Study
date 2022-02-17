// ***************************************************************
//  Copyright(c) Kingsoft
//  FileName	: apd_test.h 
//  Creator 	: page(pk)  
//  Date		: 11/19/2021 23:09:50
//  Comment	: 
// ***************************************************************	
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <ntdd1394.h>

using namespace std;

#pragma comment( lib,"winmm.lib" )

typedef NTSTATUS(CALLBACK* NTSETTIMERRESOLUTION)
(
          IN ULONG DesiredTime,
          IN BOOLEAN SetResolution,
          OUT PULONG ActualTime
);
NTSETTIMERRESOLUTION NtSetTimerResolution;

typedef NTSTATUS(CALLBACK* NTQUERYTIMERRESOLUTION)
(
         OUT PULONG MaximumTime,
         OUT PULONG MinimumTime,
         OUT PULONG CurrentTime
);
NTQUERYTIMERRESOLUTION NtQueryTimerResolution;

void QueryTimerResolution(void) {

     HMODULE hNtDll = LoadLibrary(TEXT("NtDll.dll"));
     if (hNtDll)
     {
             NtQueryTimerResolution = (NTQUERYTIMERRESOLUTION)GetProcAddress(hNtDll, "NtQueryTimerResolution");
             NtSetTimerResolution = (NTSETTIMERRESOLUTION)GetProcAddress(hNtDll, "NtSetTimerResolution");
             FreeLibrary(hNtDll);
     }
     if (NtQueryTimerResolution == NULL || NtSetTimerResolution == NULL) {
         printf("Search function failed!\n");
         return;

	}

     NTSTATUS nStatus;

     ULONG Min = 0;
     ULONG Max = 0;
     ULONG Cur = 0;
     nStatus = NtQueryTimerResolution(&Max, &Min, &Cur);

     printf("NtQueryTimerResolution -> \nMin=%lu(100ns) Max=%lu(100ns) Cur=%lu(100ns)\n", Min, Max, Cur);

     //BOOL bSetResolution = TRUE;
     //ULONG nActualTime;
     //ULONG nDesiredTime = 20064;
     //nStatus =  NtSetTimerResolution (nDesiredTime, bSetResolution,&nActualTime);

}

/*
 * 参考[1]：https://blog.csdn.net/yangshun2008/article/details/5823918
 * 参考[2]:http://yiiyee.cn/blog/2013/09/01/clock-interval/
 * 参考[3]:https://www.cnblogs.com/howie-cn/p/14955795.html
 * 小结：
 * （1）使用WaitForSingleObject(hTime, INFINITE)的INFINITE时，这个时候SetWaitableTimer第三个周期没有作用了，这个时候周期是第二个参数决定的
 * （2）默认时钟频率为16ms，怎么改SetWaitableTime都没有，参考[2]，设置时钟频率才可以timeBeginPeriod(1)
 */

int _TestSetWaitableTimer()
{
	QueryTimerResolution();
	bool bManual = FALSE;
	DWORD dw = timeBeginPeriod(1);

	DWORD timAdjust = 0;
	DWORD timeIncrement = 0;
	BOOL timeAdjustDisabled = false;
	GetSystemTimeAdjustment(&timAdjust, &timeIncrement, &timeAdjustDisabled);

	QueryTimerResolution();

	//HANDLE hTime = CreateWaitableTimer(NULL, TRUE, NULL);
	HANDLE hTime = CreateWaitableTimer(NULL, bManual, NULL);
	if (NULL == hTime)
	{
		cout << "hTime is NULL" << endl;
		return -1;
	}

	int TimeMS = 5;
	LARGE_INTEGER liDueTime;
	//liDueTime.QuadPart = -20000000;
	//liDueTime.QuadPart = -10LL * TimeMS * 1000;
	liDueTime.QuadPart = -10000000LL / 16;
	cout << "Start time = " << ::timeGetTime() << endl;
	float fPeriod = bManual ? 0 : TimeMS;
	if (!SetWaitableTimer(hTime, &liDueTime, fPeriod, NULL, NULL, FALSE))
	{
		cout << "SetWaitableTimer error" << endl;
		return -1;
	}

	while (1)
	{
		//GetSystemTimeAdjustment(&timAdjust, &timeIncrement, &timeAdjustDisabled);
		if (!bManual)
		{
			::WaitForSingleObject(hTime, INFINITE);
			DWORD dwTimeTick = ::timeGetTime();
			cout << "WaitForSingleobject succeed.Cur time = " << dwTimeTick << endl;
		}
		else
		{
			if (WaitForSingleObject(hTime, INFINITE) != WAIT_OBJECT_0)
			{
				DWORD dwTimeTick = ::timeGetTime();
				cout << "WaitForSingleObject error.Cur time = " << dwTimeTick << endl;
				return -1;
			}
			else
			{
				DWORD dwTimeTick = ::timeGetTime();
				cout << "WaitForSingleobject succeed.Cur time = " << dwTimeTick << endl;
				SetWaitableTimer(hTime, &liDueTime, 0, NULL, NULL, FALSE);
				//SetWaitableTimer(hTime, &liDueTime, 1000, NULL, NULL, FALSE);
			}
		}
	}
	CloseHandle(hTime);
	timeEndPeriod(1);
	return 0;
}

void _TestInterLockedFunction()
{
	unsigned int des = 3;
	unsigned int a, b;
	a = 2, b = 3;
	std::cout << "InterlockedCompareExchange() = " << ::InterlockedCompareExchange(&des, a, b) << endl;
	std::cout << "des = " << des << endl;
	std::cout << "a = " << a << endl;
	std::cout << "b = " << b << endl;
}

void FnTestAPI()
{
	/*for (int i = 0; i < 100000; ++i)
	{
		DWORD dwTimeTick = ::timeGetTime();
		cout << "Time: " << dwTimeTick << endl;
	}*/
	//_TestSetWaitableTimer();

	_TestInterLockedFunction();
}