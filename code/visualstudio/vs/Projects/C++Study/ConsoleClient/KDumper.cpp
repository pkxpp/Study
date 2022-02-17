#include "stdafx.h"
#include "KDumper.h"
#include "Crasheye.hpp"
#include <strsafe.h>


typedef BOOL(*pfnInitDumperExType)(const char* strProject, const char* versionEx, const char* version);
typedef void (*pfnUnInitDumperType)();
typedef BOOL(*pfnSetOnMiniDumpCreateCallBackType)(KDumper::OnMiniDumpCreateCallback pCallback);
typedef BOOL(*pfnSetConfigType)(const int nCommandType, const void* pArg);

static HMODULE gs_hDumper = NULL;
static pfnSetOnMiniDumpCreateCallBackType gs_pfnSetOnMiniDumpCreateCallBack = NULL;
static pfnSetConfigType gs_pfnSetConfig = NULL;

// 生成以当前进程所在目录为前缀的文件名全路径
BOOL GetFileWithProgramPath(const WCHAR cszFileName[], DWORD dwFullNameSize, WCHAR* pszFullName)
{
	BOOL bResult = FALSE;
	DWORD dwRetVal = 0;
	HRESULT hr = E_FAIL;

	dwRetVal = GetModuleFileNameW(NULL, pszFullName, dwFullNameSize);
	if (dwRetVal > 0 && dwRetVal < dwFullNameSize)
	{
		WCHAR* pszLastFileName = wcsrchr(pszFullName, L'\\');
		if (pszLastFileName)
		{
			hr = StringCchCopyW(pszLastFileName + 1, dwFullNameSize - (pszLastFileName - pszFullName + 1), cszFileName);
			bResult = (S_OK == hr);
		}
	}
Exit0:
	return bResult;
}

BOOL KDumper::InitKDumper(const char* pszProject, const char* pszVersionEx, const char* pszVersion)
{
	BOOL bResult = FALSE;
	
	gs_hDumper = LoadLibraryW(DUMPER_DLL);
	if (gs_hDumper)
	{
		pfnInitDumperExType pInitFunction = (pfnInitDumperExType)::GetProcAddress(gs_hDumper, "InitDumperEx");
		gs_pfnSetOnMiniDumpCreateCallBack = (pfnSetOnMiniDumpCreateCallBackType)::GetProcAddress(gs_hDumper, "SetOnMiniDumpCreateCallBack");
		gs_pfnSetConfig = (pfnSetConfigType)::GetProcAddress(gs_hDumper, "SetConfig");
		if (pInitFunction)
		{
			//bResult = pInitFunction(pszProject, pszVersionEx, pszVersion);
			Crasheye::SetURL("internal_cn");
			bResult = Crasheye::Init("174661e0", pszVersion, "");
			if (FALSE == bResult)
			{
				::FreeLibrary(gs_hDumper);
				gs_hDumper = NULL;
			}
		}
	}

	return bResult;
}

void KDumper::UnInitKDumper()
{
	if (gs_hDumper)
	{
		pfnUnInitDumperType pUnInitFunction = (pfnUnInitDumperType)::GetProcAddress(gs_hDumper, "UnInitDumper");
		if (pUnInitFunction)
		{
			Crasheye::UnInit();
		}		
		gs_pfnSetOnMiniDumpCreateCallBack = NULL;

		::FreeLibrary(gs_hDumper);
		gs_hDumper = NULL;
	}
}

BOOL KDumper::SetMiniDumpCreateCallback(OnMiniDumpCreateCallback pCallback)
{
	BOOL bResult = FALSE;

	if (gs_pfnSetOnMiniDumpCreateCallBack)
	{
		bResult= gs_pfnSetOnMiniDumpCreateCallBack(pCallback);
	}

	return bResult;
}

BOOL KDumper::SetConfig(const int nCommandType, const void* pArg)
{
	BOOL bResult = FALSE;

	if (gs_pfnSetConfig)
	{
		bResult = gs_pfnSetConfig(nCommandType, pArg);
	}

	return bResult;
}