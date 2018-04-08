#pragma once

#include <windows.h>

class KCFileFind
{
public:
	KCFileFind(void);
	~KCFileFind(void);

public:
	void First(LPCTSTR lpFileName);
	void Next();
private:
	bool m_bFound;
	HANDLE m_hFind;
	WIN32_FIND_DATA m_FindFileData;
public:
	bool IsExist(void);
	WIN32_FIND_DATA& GetCurrentFile(void);
};
