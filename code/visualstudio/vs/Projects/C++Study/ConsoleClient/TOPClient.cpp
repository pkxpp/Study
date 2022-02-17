#include "stdafx.h"
#include "TOPClient.h"

#include <iostream>
using namespace std;

TOP::VirtualClient g_VirtualClient;


class TestMemoryLeak
{
public:
	TestMemoryLeak()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(2542);
	}
} memdetector;

int _TestSetWaitableTimer()
{
	DWORD timAdjust = 0;
	DWORD timeIncrement = 0;
	BOOL timeAdjustDisabled = false;
	GetSystemTimeAdjustment(&timAdjust, &timeIncrement, &timeAdjustDisabled);

	//NtQueryTimerResolution();

	bool bManual = FALSE;
	DWORD dw = timeBeginPeriod(1);

	GetSystemTimeAdjustment(&timAdjust, &timeIncrement, &timeAdjustDisabled);

	//HANDLE hTime = CreateWaitableTimer(NULL, TRUE, NULL);
	HANDLE hTime = CreateWaitableTimer(NULL, bManual, NULL);
	if (NULL == hTime)
	{
		cout << "hTime is NULL" << endl;
		return -1;
	}

	LARGE_INTEGER liDueTime;
	//liDueTime.QuadPart = -20000000;
	int TimeMS = 10;
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
		if (!bManual)
		{
			//::WaitForSingleObject(hTime, INFINITE);
			DWORD dwTimeTick = ::timeGetTime();
			cout << "WaitForSingleobject succeed.Cur time = " << dwTimeTick << "---" << ::GetTickCount() << endl;
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

int main()
{
	_TestSetWaitableTimer();
	return 0;
}

int WINAPI WinMain(
	__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in LPSTR lpCmdLine,
	__in int nShowCmd
)
{
	int nResult = false;

	do
	{
		//EnablePrivileges();	//提升权限，不然创建链接会失败

		//timeBeginPeriod(1);

		//ERROR_BREAK(g_VirtualClient.Initialize(hInstance));
		//g_VirtualClient.Run();
		_TestSetWaitableTimer();

		nResult = true;
	} while (0);

	//timeEndPeriod(1);
	return 0;
}

namespace TOP
{
	VirtualClient* g_spTopClient = 0;
	VirtualClient::VirtualClient()
	{
		g_spTopClient = this;
	}

	VirtualClient::~VirtualClient()
	{

	}

	extern "C" int __declspec(dllexport)
	VirtualClient::Loop()
	{
		::WaitForSingleObject(m_hWaiter, INFINITE);

		while (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
			{
				return false;
			}

			::TranslateMessage(&m_msg);
			::DispatchMessage(&m_msg);
		}

		DWORD dwTimeTick = ::timeGetTime();
		return true;
	}

	int VirtualClient::SetWaiterTimer(float fFrame)
	{
		LARGE_INTEGER liDueTime;
		LONG lPeriod = (LONG)fFrame;

		liDueTime.QuadPart = -10000000LL / 16;
		if (lPeriod <= 0)
			lPeriod = 1;
		if (lPeriod > 1000)
			lPeriod = 1000;
		lPeriod = 1000 / lPeriod;

		::SetWaitableTimer(m_hWaiter, &liDueTime, lPeriod, NULL, NULL, 0);
		return true;
	}

	int VirtualClient::CreateWaiterTimer()
	{
		int nResult = false;

		do
		{
			m_hWaiter = ::CreateWaitableTimer(NULL, FALSE, NULL);

			nResult = true;
		} while (0);

		return nResult;
	}

	int VirtualClient::DestroyWaiterTimer()
	{
		if (m_hWaiter)
		{
			::CloseHandle(m_hWaiter);
			m_hWaiter = 0;
		}
		return true;
	}

}