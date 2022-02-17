#include "stdafx.h"
#include "console.h"

Console::Console()
	: m_bAllocConsole(FALSE)
	, m_pOutput(NULL)
{

}

Console::~Console()
{

}

int Console::Create()
{
	bool bResult = false;
	bool bRetCode = false;

	if (!m_bAllocConsole)
	{
		bRetCode = ::AllocConsole();
		KG_PROCESS_ERROR(bRetCode);

		m_bAllocConsole = TRUE;
	}

	if (!m_pOutput)
	{
		freopen_s(&m_pOutput, "CONOUT$", "w+", stdout);
		KG_PROCESS_ERROR(m_pOutput);
	}

	bResult = true;
Exit0:
	if (!bResult)
		Destroy();
	return bResult;
}

void Console::Destroy()
{
	if (m_bAllocConsole)
	{
		::FreeConsole();
		m_bAllocConsole = FALSE;
	}

	if (m_pOutput)
	{
		fclose(m_pOutput);
		m_pOutput = NULL;
	}
}

/*
int Console::Read(char* pRetBuffer, DWORD dwNumberOfBytersToRead, DWORD* pdwRetNumberOfBytesRead)
{
	int nResult = false;
	BOOL bRetCode = FALSE;
	DWORD dwAvail = 0;

	do
	{
		ERROR_BREAK(pRetBuffer);
		ERROR_BREAK(pdwRetNumberOfBytesRead);

		*pdwRetNumberOfBytesRead = 0;

		bRetCode = ::PeekNamedPipe(m_hReadPipe, NULL, 0, NULL, &dwAvail, NULL);
		ERROR_BREAK(bRetCode);

		if (dwAvail)
		{
			bRetCode = ::ReadFile(m_hReadPipe, pRetBuffer, dwNumberOfBytersToRead, pdwRetNumberOfBytesRead, NULL);
			ERROR_BREAK(bRetCode);
		}

		nResult = true;
	} while (0);

	return nResult;
}*/

/*
int Console::Read(char* pRetBuffer, DWORD dwNumberOfBytersToRead, DWORD* pdwRetNumberOfBytesRead)
{
	int nResult = false;
	BOOL bRetCode = FALSE;
	DWORD dwAvail = 0;
	INPUT_RECORD InputRecord;

	do
	{
		ERROR_BREAK(pRetBuffer);
		ERROR_BREAK(pdwRetNumberOfBytesRead);

		while (::PeekConsoleInput(m_hConsoleInput, &InputRecord, 1, pdwRetNumberOfBytesRead))
		{
			if ((*pdwRetNumberOfBytesRead) == 0)
				break;

			bRetCode = ::ReadConsoleInput(m_hConsoleInput, &InputRecord, 1, pdwRetNumberOfBytesRead);
			ERROR_BREAK(bRetCode);

			if (InputRecord.EventType == KEY_EVENT && InputRecord.Event.KeyEvent.bKeyDown)
			{
				//printf ("%c", InputRecord.Event.KeyEvent.uChar.AsciiChar);
			}
		}

		nResult = true;
	} while (0);

	return nResult;
}*/
