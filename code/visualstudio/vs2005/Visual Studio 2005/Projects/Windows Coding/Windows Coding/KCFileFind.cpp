#include "KCFileFind.h"

KCFileFind::KCFileFind(void)
: m_bFound(false)
{
	m_hFind = INVALID_HANDLE_VALUE;
	memset(&m_FindFileData, 0, sizeof(m_FindFileData));
}

KCFileFind::~KCFileFind(void)
{
	FindClose(m_hFind);
}

void KCFileFind::First(LPCTSTR lpFileName)
{
	m_hFind = ::FindFirstFile(lpFileName, &m_FindFileData);
	if( m_hFind != INVALID_HANDLE_VALUE)
		m_bFound = true;
	else
		m_bFound = false;
}

void KCFileFind::Next()
{
	m_bFound = ::FindNextFile(m_hFind, &m_FindFileData) ? true: false;
}

bool KCFileFind::IsExist(void)
{
	return m_bFound;
}

WIN32_FIND_DATA& KCFileFind::GetCurrentFile(void)
{
	//TODO: insert return statement here
	return m_FindFileData;
}
