// CTPSTOCK.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ZQMD.h"
#include "ZQTD.h"

int main(int argc, char* argv[])
{
	CZQMD * pMdHandler=new CZQMD();
	pMdHandler->Init();

	CZQTD * pTdHandler=new CZQTD();
	pTdHandler->Init();

	HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);	
	WaitForSingleObject(g_hEvent, INFINITE);

	return 0;
}

