#include <iostream>
#include <iomanip>
#include <shlwapi.h>
#include <string.h>
#include <strstream>
#include <filesystem>
#include <tchar.h>
#include <time.h>
#include <PathCch.h>
#include <direct.h>
using namespace std;
//namespace fs = std::filesystem;


#pragma comment(lib, "shlwapi.lib") 

#define NELEMS(x)		(sizeof((x))/(sizeof(((x)[0]))))

#include <stdio.h>
#include <string.h>

#pragma warning(disable : 4995)

int KG_SplitPath(
	char const* _FullPath,
	char* _Drive,
	size_t      _DriveCount,
	char* _Dir,
	size_t      _DirCount,
	char* _Filename,
	size_t      _FilenameCount,
	char* _Ext,
	size_t      _ExtCount
);

BOOL KG_GetDirectoryAndFileName(
	const char* pcszFilePath,              /*in*/
	char* pszRetDirectoryBuffer,     /*out_opt*/
	size_t         uDirectoryBufferLength,       /*in_opt*/
	char* pszRetFileNameBuffer,      /*out_opt*/
	size_t         uFileNameBufferLength         /*in_opt*/
);

BOOL KG_PathCanonicalizeA(
	char* pszRetFilePathBuffer,				/*out*/
	size_t			uPathBufferLength,					/*out_opt*/
	const char* pcszFilePath,						/*in*/
	size_t			uPathLength							/*in_opt*/
);

void splitname1(const char* szfullfilename, char* szpathname, char* szfilename, char* szextname)
{
	int i, j;

	i = 0;
	while (szfullfilename[i] != '\0')
		i++;
	while (szfullfilename[i] != '\\')
		i--;

	for (j = 0; j < i; j++)
	{
		szpathname[j] = szfullfilename[j];
	}
	szpathname[j] = '\0';

	j = 0;
	i++;
	while ((szfilename[j] = szfullfilename[i]) != '\0')
	{
		i++;
		j++;
	}

	while (szfullfilename[i] != '.')
		i--;

	j = 0;
	i++;
	while ((szextname[j] = szfullfilename[i]) != '\0')
	{
		j++;
		i++;
	}
}

void splitname(const char* szfullfilename, char* szpathname, char* szfilename, char* szextname)
{
	int i, j;

	i = 0;
	while (szfullfilename[i] != '\0')
		i++;
	while (szfullfilename[i] != '.')
		i--;

	j = 0;
	i++;
	while ((szextname[j] = szfullfilename[i]) != '\0')
	{
		i++;
		j++;
	}
	i -= j;
	while (szfullfilename[i] != '\\')
		i--;

	for (j = 0; j <= i; j++)
	{
		szpathname[j] = szfullfilename[j];
	}
	szpathname[j] = '\0';

	j = 0;
	i++;
	while ((szfilename[j] = szfullfilename[i]) != '\0')
	{
		i++;
		j++;
	}
}

void TestJapaneseCode()
{
	//char* p = setlocale(LC_ALL, "chs");
	char* p = setlocale(LC_ALL, "");
	//cout << p << endl;
	_get_current_locale();

	{
		//char szFullPath[] = "data\\source\\maps\\empty_launch\\empty_launch.jsonmap";
		char szFullPath[] = "data\\平野孤鸿\\平野孤鸿.jsonmap";
		char szDriver[MAX_PATH];
		char szDir[MAX_PATH];
		char szFileName[MAX_PATH] = {0};
		char szExt[MAX_PATH];
		const int TEST_NUM = 1;

		clock_t tStart = clock();
		for (int i = 0; i < TEST_NUM; ++i)
		{
			errno_t e = _splitpath_s(szFullPath, szDriver, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);
		}
		clock_t tEnd = clock();
		cout << "_splitpath_s cost time: " << tEnd - tStart << endl;
		tStart = clock();
		for (int i = 0; i < TEST_NUM; ++i)
		{
			errno_t e = KG_SplitPath(szFullPath, szDriver, MAX_PATH, NULL, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);
		}
		tEnd = clock();
		cout << "KG_SplitPath cost time: " << tEnd - tStart << endl;

		cout << "szCurrentDir = " << szFullPath << endl;
		cout << "szDriver = " << szDriver << endl;
		cout << "szDir = " << szDir << endl;
		cout << "szFileName = " << szFileName << endl;
		cout << "szExt = " << szExt << endl;
	}

	//const int MAX_PATH = 256;
	if(false)
	{
		//TCHAR szFullPath[] = _T("data\\平野孤鸿\\平野孤鸿.jsonmap");
		TCHAR szFullPath[] = _T("data\\source\\maps\\empty_launch\\empty_launch.jsonmap");

		//char szCurrentDir[] = "data\\鸿.txt";
		//cout << "sizeof(Dir) = " << sizeof(wszFullPath) << endl;

		//for (int i = 0; i < sizeof(wszFullPath); i++)
		//{
		//	cout << hex << (unsigned int)(unsigned char)szFullPath[i] << endl;
		//}

		//char* pfind = strstr(wszFullPath, ".jsonmap");
		//cout << "pfind = " << pfind << endl;

		TCHAR szDriver[MAX_PATH];
		TCHAR szDir[MAX_PATH];
		TCHAR szFileName[MAX_PATH];
		TCHAR szExt[MAX_PATH];
		//errno_t e = _tsplitpath_s(szFullPath, szDriver, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);
		//errno_t e = KG_SplitPath(szFullPath, szDriver, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);
#ifdef _UNICODE
		/*wcout << "szCurrentDir = " << szFullPath << endl;
		wcout << "szDriver = " << szDriver << endl;
		wcout << "szDir = " << szDir << endl;
		wcout << "szFileName = " << szFileName << endl;
		wcout << "szExt = " << szExt << endl;*/
		wprintf(L"wszDir = %s\n", szDir);
		wprintf(L"wszFileName = %s\n", szFileName);
		wprintf(L"wszExt = %ls\n", szExt);
#else
		//errno_t e = KG_SplitPath(szFullPath, szDriver, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, szExt, MAX_PATH);
		//errno_t e = KG_SplitPath(szFullPath, szDriver, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, NULL, 0);
		cout << "szCurrentDir = " << szFullPath << endl;
		cout << "szDriver = " << szDriver << endl;
		cout << "szDir = " << szDir << endl;
		cout << "szFileName = " << szFileName << endl;
		cout << "szExt = " << szExt << endl;
#endif
	}
	

	// wchar
	{
		//wchar_t wszFullPath[] = L"data\\平野孤鸿\\平野孤鸿.jsonmap";
		//cout << "sizeof(wszFullPath) = " << sizeof(wszFullPath) << endl;
		//for (int i = 0; i < sizeof(wszFullPath); i++)
		//{
		//	//cout << hex << (unsigned int)(unsigned char)wszFullPath[i] << endl;
		//}
		//wchar_t wszDriver[MAX_PATH];
		//wchar_t wszDir[MAX_PATH];
		//wchar_t wszFileName[MAX_PATH];
		//wchar_t wszExt[MAX_PATH];
		//errno_t e = _wsplitpath_s(wszFullPath, wszDriver, MAX_PATH, wszDir, MAX_PATH, wszFileName, MAX_PATH, wszExt, MAX_PATH);
		//wprintf(L"wszFullPath = %s\n", wszFullPath);
		///*wcout << "wszCurrentDir = " << wszFullPath << endl;
		//wcout << "wszDriver = " << wszDriver << endl;
		//wcout << "wszDir = " << wszDir << endl;*/
		////wcout << "wszFileName = " << wszFileName << endl;
		//wprintf(L"wszDir = %s\n", wszDir);
		//wprintf(L"wszFileName = %s\n", wszFileName);
		//wprintf(L"wszExt = %ls\n", wszExt);
	}
	

	//char* pFileName = PathFindFileNameA(szCurrentDir);
	//char* pExt = PathFindExtensionA(szCurrentDir);
	//cout << "pFileName = " << pFileName << endl;
	//cout << "pExt = " << pExt << endl;

	//splitname1(szCurrentDir, szDir, szFileName, szExt);
	//cout << "szCurrentDir = " << szCurrentDir << endl;
	//cout << "szDriver = " << szDriver << endl;
	//cout << "szDir = " << szDir << endl;
	//cout << "szFileName = " << szFileName << endl;
	//cout << "szExt = " << szExt << endl;、

	//////////////////////////////////////////////////////////////////////////
	// pathremovefilespec
	{
		char szFullPath[] = "data\\source\\maps_source\\树\\s_xb多枝枯树003_010.mtl";
		char szDir[MAX_PATH];
		char szName[MAX_PATH];
		//strcpy_s(szDir, szFullPath);
		//PathRemoveFileSpecA(szDir);
		//PathCchRemoveFileSpec(szDir, MAX_PATH);
		KG_GetDirectoryAndFileName(szFullPath, szDir, MAX_PATH, szName, MAX_PATH);
		cout << "szFullPath = " << szFullPath << endl;
		cout << "szDir = " << szDir << endl;

		//realpath();
	}

	//////////////////////////////////////////////////////////////////////////
	{
		char szRelativePath[] = "data\\source\\maps_source\\树\\..\\texture\\sp_西北枯树001_001.tga";
		char szAbsolutePath[MAX_PATH] = {0};
		//realpath
		// 包括盘符
		PathCombineA(szAbsolutePath, "a\\b\\", "..\c");
		GetFullPathNameA(szRelativePath, MAX_PATH, szAbsolutePath, NULL);
		cout << "szRelativePath = " << szRelativePath << endl;
		cout << "szAbsolutePath = " << szAbsolutePath << endl;
		// 包括盘符
		//_fullpath(szAbsolutePath, szRelativePath, MAX_PATH);

		//realpath(szRelativePath, szAbsolutePath);
		PathCanonicalizeA(szAbsolutePath, szRelativePath);
		cout << "szAbsolutePath = " << szAbsolutePath << endl;
		//strcpy_s(szAbsolutePath, szRelativePath);

		memset(szAbsolutePath, 0, MAX_PATH);
		KG_PathCanonicalizeA(szAbsolutePath, MAX_PATH, szRelativePath, MAX_PATH);
		cout << "szAbsolutePath = " << szAbsolutePath << endl;
		memset(szAbsolutePath, 0, MAX_PATH);
		char szTest[MAX_PATH] = "a/b/../c";
		KG_PathCanonicalizeA(szAbsolutePath, MAX_PATH, szTest, sizeof(szTest));
		KG_PathCanonicalizeA(szTest, MAX_PATH, szTest, sizeof(szTest));
		cout << "a/b/../c = " << szAbsolutePath << endl;
	}
}

void TestShaderHash();

int main()
{
	//system("chcp 65001 && cls");
	//const char* szChinese = "特效";
	//cout << hex << szChinese << endl;

	//const char* szChinese = "\xCC\xD8";
	//cout << szChinese << endl;

	//char szOut[1024] = {0};
	//int nSize1 = sizeof(szChinese);
	//int nSize2 = sizeof(szChinese[0]);
	//int nLen = NELEMS(szChinese);
	//for (int i = 0; i < nLen; ++i)
	//{
	//	cout << hex << setfill(' ') << (unsigned short)szChinese[i] << endl;
	//	//printf("%x", szChinese[i]);
	//	szOut[i] = tolower(szChinese[i]);
	//	cout << hex << setfill(' ') << (unsigned short)szOut[i] << endl;
	//}

	//cout << hex << szOut << endl;
	//TestJapaneseCode();
	TestShaderHash();
	getchar();
	return 0;
}

// 小结：tolower应该是需要系统在unicode环境下才有用，第一句system("chcp 65001 && cls");并木有生效


int KG_SplitPath(
	char const* _FullPath,
	char* _Drive,
	size_t      _DriveCount,
	char* _Dir,
	size_t      _DirCount,
	char* _Filename,
	size_t      _FilenameCount,
	char* _Ext,
	size_t      _ExtCount
)
{
#if 1
	if (!_FullPath)
		return -1;

	// no process drive
	if (_Drive) _Drive[0] = '\0';

	size_t nLen = strlen(_FullPath);
	if (nLen >= MAX_PATH)
		nLen = MAX_PATH - 1;

	char FullPath[MAX_PATH] = { 0 };
	memcpy_s(FullPath, MAX_PATH, _FullPath, nLen);

	// normalize splash
	for (int i = 0; i < nLen; ++i)
	{
		if (FullPath[i] == '/') FullPath[i] = '\\';
	}

	if ('\\' == FullPath[nLen - 1])
	{
		if (_Dir) strcpy_s(_Dir, _DirCount, FullPath);
		if (_Filename) _Filename[0] = '\0';
		if (_Ext) _Ext[0] = '\0';
		return 0;
	}

	auto _SplitWholeName = [](const char* szWholeName, char* szFileName, size_t nFileNameCount, char* szExt, size_t nExtCount)
	{
		const char* cpExt = strrchr(szWholeName, '.');
		if (!cpExt)
		{
			if (szFileName) strcpy_s(szFileName, nFileNameCount, szWholeName);
			if (szExt) szExt[0] = '\0';
			return;
		}

		if (szExt) strcpy_s(szExt, nExtCount, cpExt);
		if (szFileName)
		{
			rsize_t nLen = cpExt - szWholeName;
			if (nLen >= nFileNameCount)
				nLen = nFileNameCount - 1;
			strncpy_s(szFileName, nLen + 1, szWholeName, nLen);
			szFileName[nLen] = '\0';
		}
	};

	const char* pWholeName = strrchr(FullPath, '\\');
	if (!pWholeName)
	{
		_SplitWholeName(FullPath, _Filename, _FilenameCount, _Ext, _ExtCount);
		if (_Dir) _Dir[0] = '\0';
		return 0;
	}

	++pWholeName;
	_SplitWholeName(pWholeName, _Filename, _FilenameCount, _Ext, _ExtCount);

	if (_Dir)
	{
		nLen = pWholeName - FullPath;
		if (nLen >= _DirCount)
			nLen = _DirCount - 1;
		strncpy_s(_Dir, nLen + 1, FullPath, nLen);
		_Dir[nLen] = '\0';
	}

	return 0;
#else
	return _splitpath_s(_FullPath, _Drive, _DirCount, _Dir, _DirCount, _Filename, _FilenameCount, _Ext, _ExtCount);
#endif
	}

int KG_SplitPath_Old(
	char const* _FullPath,
	char* _Drive,
	size_t      _DriveCount,
	char* _Dir,
	size_t      _DirCount,
	char* _Filename,
	size_t      _FilenameCount,
	char* _Ext,
	size_t      _ExtCount
)
{
	if (_Drive)
	{
		_Drive[0] = '\0';
	}

	if (NULL == _FullPath)
	{
		if (_Dir) _Dir[0] = '\0';
		if (_Filename) _Filename[0] = '\0';
		if (_Ext) _Ext[0] = '\0';
		return 0;
	}

	int nLen = strlen(_FullPath);
	char FullPath[MAX_PATH] = { 0 };
	memcpy_s(FullPath, MAX_PATH, _FullPath, nLen);

	for (int i = 0; i < nLen; ++i)
	{
		if (FullPath[i] == '/')
		{
			FullPath[i] = '\\';
		}
	}

	if ('\\' == FullPath[nLen])
	{
		strcpy_s(_Dir, MAX_PATH, FullPath);
		if (_Filename) _Filename[0] = '\0';
		if (_Ext) _Ext[0] = '\0';
		return 0;
	}

	auto _SplitWholeName = [](const char* whole_name, char* fname, char* ext)
	{
		const char* p_ext = strrchr(whole_name, '.');
		if (NULL != p_ext)
		{
			if (ext) strcpy_s(ext, MAX_PATH, p_ext);
			if (fname)
			{
				int nLen = p_ext - whole_name;
				strncpy_s(fname, nLen + 1, whole_name, nLen);
				fname[nLen] = '\0';
			}
		}
		else
		{
			if (ext) ext[0] = '\0';
			if (fname) strcpy_s(fname, MAX_PATH, whole_name);
		}
	};

	const char* pWholeName = strrchr(FullPath, '\\');
	if (pWholeName)
	{
		pWholeName++;
		_SplitWholeName(pWholeName, _Filename, _Ext);

		if (_Dir)
		{
			nLen = pWholeName - FullPath;
			strncpy_s(_Dir, nLen + 1, FullPath, nLen);
			_Dir[nLen] = '\0';
		}
	}
	else
	{
		_SplitWholeName(FullPath, _Filename, _Ext);
		if (_Dir) _Dir[0] = '\0';
	}

	return 0;
}

//char*
//dirname(char* path)
//{
//	static const char dot[] = ".";
//	char* last_slash;
//	/* Find last '/'.  */
//	last_slash = path != NULL ? strrchr(path, '/') : NULL;
//	if (last_slash != NULL && last_slash != path && last_slash[1] == '\0')
//	{
//		/* Determine whether all remaining characters are slashes.  */
//		char* runp;
//		for (runp = last_slash; runp != path; --runp)
//			if (runp[-1] != '/')
//				break;
//		/* The '/' is the last character, we have to look further.  */
//		if (runp != path)
//			last_slash = memrchr(path, '/', runp - path);
//	}
//	if (last_slash != NULL)
//	{
//		/* Determine whether all remaining characters are slashes.  */
//		char* runp;
//		for (runp = last_slash; runp != path; --runp)
//			if (runp[-1] != '/')
//				break;
//		/* Terminate the path.  */
//		if (runp == path)
//		{
//			/* The last slash is the first character in the string.  We have to
//			   return "/".  As a special case we have to return "//" if there
//			   are exactly two slashes at the beginning of the string.  See
//			   XBD 4.10 Path Name Resolution for more information.  */
//			if (last_slash == path + 1)
//				++last_slash;
//			else
//				last_slash = path + 1;
//		}
//		else
//			last_slash = runp;
//		last_slash[0] = '\0';
//	}
//	else
//		/* This assignment is ill-designed but the XPG specs require to
//		   return a string containing "." in any case no directory part is
//		   found and so a static and constant string is required.  */
//		path = (char*)dot;
//	return path;
//}


BOOL KG_GetDirectoryAndFileName(
	const char* pcszFilePath,              /*in*/
	char* pszRetDirectoryBuffer,     /*out_opt*/
	size_t         uDirectoryBufferLength,       /*in_opt*/
	char* pszRetFileNameBuffer,      /*out_opt*/
	size_t         uFileNameBufferLength         /*in_opt*/
)
{
	int nResult = false;
	size_t  uFilePathLength = 0;
	size_t  uLastSplashPos = 0;
	size_t uDirectoryNameLength = 0;
	int i = 0;

	//KGLOG_ASSERT_EXIT(pcszFilePath);
	//KGLOG_ASSERT_EXIT(pcszFilePath[0]);
	/*KGLOG_ASSERT_EXIT(
		(pszRetDirectoryBuffer == NULL && uDirectoryBufferLength == 0) ||
		(pszRetDirectoryBuffer != NULL && uDirectoryBufferLength > 0)
	);*/
	/*KGLOG_ASSERT_EXIT(
		(pszRetFileNameBuffer == NULL && uFileNameBufferLength == 0) ||
		(pszRetFileNameBuffer != NULL && uFileNameBufferLength > 0)
	);*/

	uFilePathLength = strlen(pcszFilePath);
	//KGLOG_ASSERT_EXIT(uFilePathLength > 0);

	for (i = (int)(uFilePathLength - 1); i >= 0; --i)
	{
		if (pcszFilePath[i] == L'\\' || pcszFilePath[i] == L'/')
		{
			uLastSplashPos = i;
			break;
		}
	}
	// Treat it as error if the file path have no splash character,
	//KGLOG_PROCESS_ERROR(i != 0);

	// The last splash should not appear at the begin or end of the file path.
	//KGLOG_PROCESS_ERROR(uLastSplashPos > 0 && (uLastSplashPos < uFilePathLength - 1));

	uDirectoryNameLength = uLastSplashPos;

	if (pszRetDirectoryBuffer)
	{
		//KGLOG_PROCESS_ERROR(uDirectoryNameLength < uDirectoryBufferLength);
		memcpy(pszRetDirectoryBuffer, pcszFilePath, uDirectoryNameLength);
		pszRetDirectoryBuffer[uDirectoryNameLength] = L'\0';
	}

	if (pszRetFileNameBuffer)
	{
		size_t uFileNameLength = uFilePathLength - uLastSplashPos - 1;

		//KGLOG_PROCESS_ERROR(uFileNameLength < uFileNameBufferLength);
		memcpy(pszRetFileNameBuffer, pcszFilePath + uDirectoryNameLength + 1, uFileNameLength);
		pszRetFileNameBuffer[uFileNameLength] = L'\0';
	}

	nResult = true;
Exit0:
	return nResult;
}

#ifdef WIN32
#define IS_SLASH(s) ((s == '/') || (s == '\\'))
#else
#define IS_SLASH(s) (s == '/')
#endif
#define IS_DOT(s) (s == '.')
#define IS_DOT_DOT(s, t) (s == '.' && t == '.')

////////////////////////////////////////////////////////////////////////////////
// _MSC_VER warning C4127: conditional expression is constant
// use this macro instead "while (false)", you can enjoy the level 4 warning ^_^
#define WHILE_FALSE_NO_WARNING          \
    TEMP_DISABLE_WARNING(4127, while (false))

#define WHILE_TRUE_NO_WARNING          \
    TEMP_DISABLE_WARNING(4127, while (true))

#ifdef _MSC_VER
#define ASSERT(Condition)  _ASSERTE(Condition)
#endif

#define KGLOG_ASSERT_EXIT(Condition)    \
do                                      \
{                                       \
    if (!(Condition))                   \
    {                                   \
        printf("KGLOG_ASSERT_EXIT(%s) at line %u in %s", \
            #Condition, __LINE__, __FUNCTION__  \
        );                              \
        ASSERT(Condition);              \
        goto Exit0;                     \
    }                                   \
} WHILE_FALSE_NO_WARNING

////////////////////////////////////////////////////////////////////////////////
/**
\brief      Simplifies a path by removing navigation elements such as "." and ".." to produce a direct, well-formed path.
\param[out] pszRetFilePathBuffer   A pointer to a buffer that receives the result file path.
\param[in]  uPathBufferLength      Length of buffer point by pszRetFilePathBuffer, in char count.
								   This parameter must be 0, if pszRetFilePathBuffer equals NULL.
\param[in]  pcszFilePath           The file path.
\param[in]  uPathLength            Length of buffer point by pcszFilePath, in char count.
								   This parameter must be 0, if pcszFilePath equals NULL.

example:
	If you specify "\\a\b\\..\c" then:

\return true if successful, false if failed.
*/
BOOL KG_PathCanonicalizeA(
	char*			pszRetFilePathBuffer,				/*out*/
	size_t			uPathBufferLength,					/*out_opt*/
	const char*		pcszFilePath,						/*in*/
	size_t			uPathLength							/*in_opt*/
)
{

	//KGLOG_ASSERT_EXIT(pcszFilePath);
	//KGLOG_ASSERT_EXIT(pcszFilePath[0]);
	//KGLOG_ASSERT_EXIT(
	//	(pszRetDirectoryBuffer == NULL && uDirectoryBufferLength == 0) ||
	//	(pszRetDirectoryBuffer != NULL && uDirectoryBufferLength > 0)
	//);


	int nSrc = 0, nDst = 0;
	for (; nSrc < uPathLength && nDst < uPathBufferLength && pcszFilePath[nSrc] && (pcszFilePath[nSrc] != '.'); ++nSrc)
	{
		pszRetFilePathBuffer[nDst++] = pcszFilePath[nSrc];
	}

	auto fnRollback = [&]() {
		if (nDst >= 0)
		{
			while (nDst >= 0 && !IS_SLASH(pszRetFilePathBuffer[nDst]))
				--nDst;
			++nDst;
		}
		else
		{
			nDst = 0;
		}
	};
	while (pcszFilePath[nSrc] != '\0' && nSrc < uPathLength && nDst < uPathBufferLength) 
	{
		if (IS_DOT(pcszFilePath[nSrc]) && IS_SLASH(pcszFilePath[nSrc + 1]) && (nSrc == 0 || IS_SLASH(pcszFilePath[nSrc - 1])))
		{
			nSrc += 2;
		}	
		else
		{
			// sample: a/../b
			if (IS_DOT_DOT(pcszFilePath[nSrc], pcszFilePath[nSrc + 1]) && IS_SLASH(pcszFilePath[nSrc + 2]) && (nSrc == 0 || IS_SLASH(pcszFilePath[nSrc - 1])))
			{
				nSrc += 3;
				nDst -= 2;
				fnRollback();
			}
			else
			{
				pszRetFilePathBuffer[nDst++] = pcszFilePath[nSrc++];
			}
		}
			
	}

	if (nDst == 1 && IS_DOT(pszRetFilePathBuffer[0]))
		--nDst;

	else if (nDst > 1 && IS_DOT(pszRetFilePathBuffer[nDst - 1]) && IS_SLASH(pszRetFilePathBuffer[nDst - 2]))
		--nDst;

	pszRetFilePathBuffer[nDst] = '\0';


	if (nDst == 2 && IS_DOT_DOT(pszRetFilePathBuffer[0], pszRetFilePathBuffer[1]))
	{
		pszRetFilePathBuffer[0] = '\0';
	}
	else if (nDst > 2 && IS_DOT_DOT(pszRetFilePathBuffer[nDst - 1], pszRetFilePathBuffer[nDst - 2]) && IS_SLASH(pszRetFilePathBuffer[nDst - 3])) 
	{
		nDst -= 4;
		fnRollback();
		pszRetFilePathBuffer[nDst] = '\0';

	}
Exit0:
	return true;
}

void TestShaderHash()
{
	typedef struct _GUID {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	} GUID;

	auto KG_BKDRHashForBuffer64Bit = [](const char* pcszString, int nLength)
	{
		uint64_t ullHash = 0;
		for (int i = 0; i < nLength; i++)
		{
			//ullHash = ullHash * 131 + *pcwszString++;
			ullHash = (ullHash << 7) + (ullHash << 1) + ullHash + *pcszString++;              //ullHash移位7位再加上ullHash移位1位在加上ullHash即131倍，在加上pcszString取值后再往后移一位
		}

		return ullHash;
	};

	auto KG3D_GUIDTOString = [](const GUID& uuid, char* pszBuffer, unsigned uBufSize)
	{
		BOOL nResult = 0;
		BOOL nRetCode = 0;

		nRetCode = sprintf_s(pszBuffer, uBufSize, "{%08x-%04hx-%04hx-%02x%02x-%02x%02x%02x%02x%02x%02x}",
			uuid.Data1,
			uuid.Data2, uuid.Data3,
			uuid.Data4[0], uuid.Data4[1],
			uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]
		);
		pszBuffer[nRetCode] = '\0';

		nResult = 1;
	};

	//std::string strPath = "data\\source\\maps_source\\material\\长歌门幻影2_1.fx5;1;shader_mode=5,shader_x11_mtlsys=,simplify_lighting=,";
	std::string strPath = "data\\material\\Material\\Mtl_Water\\Mtl_WaterFall_Stream_lod2.fx5;1;shader_mode=5,shader_x11_mtlsys=,simplify_lighting=,";
	auto fnTest = [&](std::string strPath)
	{
		uint64_t uID = KG_BKDRHashForBuffer64Bit(strPath.c_str(), strPath.length());
		GUID guid;
		memcpy_s(&guid, sizeof(GUID), &uID, sizeof(UINT64));
		char szGuid[256];
		KG3D_GUIDTOString(guid, szGuid, 256);
		printf("guid = %s\n", szGuid);
	};
	/*fnTest(strPath);
	strPath = "data\\source\\maps_source\\material\\长歌门幻影2_1.fx5;1;shader_mode=5,shader_x11_mtlsys=,simplify_lighting=,";
	fnTest(strPath);*/
	strPath = "data\\material\\shader\\mtl_water\\mtl_underwater.fx5;20;shader_mode=5,shader_x11_mtlsys=,";
	fnTest(strPath);
	strPath = "data\\material\\shader\\mtl_water\\Mtl_UnderWater.fx5;20;shader_mode=5,shader_x11_mtlsys=,";
	fnTest(strPath);
	strPath = "data\\source\\maps_source\\material\\长歌门幻影2_1.fx5;1;SHADER_MODE=5,SHADER_X11_MTLSYS=,SIMPLIFY_LIGHTING=,";
	fnTest(strPath);

	auto KG3D_NormalizePath = [](const char* pcszStr, char* pcszRetStr, size_t uRetBufferSize)
	{
		BOOL bResult = FALSE;

		//KG_PROCESS_ERROR(pcszStr);
		//KG_PROCESS_ERROR(pcszRetStr);

		strncpy_s(pcszRetStr, uRetBufferSize, pcszStr, _TRUNCATE);
		pcszRetStr[MAX_PATH - 1] = '\0';

		unsigned uStrLen = (unsigned)strlen(pcszRetStr);

		// encode error when in unicode system, replace CharLowerBuffA with LCMapStringA
		//CharLowerBuffA(pcszRetStr, uStrLen);
		const LCID cnLocale = LocaleNameToLCID(L"zh-CN", 0);
		LCMapStringA(cnLocale, LCMAP_LOWERCASE, pcszRetStr, uStrLen, pcszRetStr, uStrLen);
		//_NormalizeSplash(pcszRetStr);

		bResult = TRUE;
	};
	char szStandard[MAX_PATH] = { 0 };
	KG3D_NormalizePath(strPath.c_str(), szStandard, MAX_PATH);
	printf("convert to: %s\n", szStandard);
}