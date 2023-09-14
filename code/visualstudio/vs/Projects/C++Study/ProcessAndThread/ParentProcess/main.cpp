#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <iostream>
#include "../Cache.h"

#define BUFSIZE 4096 
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
#define CHILD_NUM 3

HANDLE g_hChildStd_IN_Rd[CHILD_NUM] = { NULL };
HANDLE g_hChildStd_IN_Wr[CHILD_NUM] = { NULL };
HANDLE g_hChildStd_OUT_Rd[CHILD_NUM] = { NULL };
HANDLE g_hChildStd_OUT_Wr[CHILD_NUM] = { NULL };

HANDLE g_hInputFile = NULL;

HANDLE m_hFile;				//文件句柄
HANDLE m_hMapping;			//内存映射句柄
void* m_pBase = nullptr;	//内存映射的地址

bool CreateMemoryMapping();	//创建内存映射
//void my_FileMappingWriteData();	//写内容
bool ClearMapping();		//关闭内存映射
void CreateChildProcess(int nIndex);
void WriteToPipe(int nIndex);
void ReadFromPipe(int nIndex);
void ErrorExit(PTSTR);

int _tmain(int argc, TCHAR* argv[])
{
    SECURITY_ATTRIBUTES saAttr;

    printf("\n->Start of parent execution.\n");

    // Set the bInheritHandle flag so pipe handles are inherited. 

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT. 
    for (int i = 0; i < CHILD_NUM; ++i)
    {
        if (!CreatePipe(&g_hChildStd_OUT_Rd[i], &g_hChildStd_OUT_Wr[i], &saAttr, 0))
            ErrorExit((PTSTR)TEXT("StdoutRd CreatePipe"));

        // Ensure the read handle to the pipe for STDOUT is not inherited.

        if (!SetHandleInformation(g_hChildStd_OUT_Rd[i], HANDLE_FLAG_INHERIT, 0))
            ErrorExit((PTSTR)TEXT("Stdout SetHandleInformation"));

        // Create a pipe for the child process's STDIN. 

        if (!CreatePipe(&g_hChildStd_IN_Rd[i], &g_hChildStd_IN_Wr[i], &saAttr, 0))
            ErrorExit((PTSTR)TEXT("Stdin CreatePipe"));

        // Ensure the write handle to the pipe for STDIN is not inherited. 

        if (!SetHandleInformation(g_hChildStd_IN_Wr[i], HANDLE_FLAG_INHERIT, 0))
            ErrorExit((PTSTR)TEXT("Stdin SetHandleInformation"));
    }
    
    // Get a handle to an input file for the parent. 
    // This example assumes a plain text file and uses string output to verify data flow. 

    if (argc == 1)
        ErrorExit((PTSTR)TEXT("Please specify an input file.\n"));

    g_hInputFile = CreateFile(
        argv[1],
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_READONLY,
        NULL);

    if (g_hInputFile == INVALID_HANDLE_VALUE)
        ErrorExit((PTSTR)TEXT("CreateFile"));

    // Create the child process. 
    CreateMemoryMapping();
    for (int i = 1; i < CHILD_NUM; ++i)
    {
        CreateChildProcess(i);

        // Write to the pipe that is the standard input for a child process. 
    // Data is written to the pipe's buffers, so it is not necessary to wait
    // until the child process is running before writing data.

        WriteToPipe(i);
        printf("\n->Contents of %S written to child STDIN pipe.\n", argv[1]);

        // Read from pipe that is the standard output for child process. 

        printf("\n->Contents of child process STDOUT:\n\n");
        ReadFromPipe(i);

        printf("\n->End of parent execution.\n");

        // The remaining open handles are cleaned up when this process terminates. 
        // To avoid resource leaks in a larger application, close handles explicitly. 
    }

    return 0;
}

void CreateChildProcess(int nIndex)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
    TCHAR szCmdline[] = TEXT("child");
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure. 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr[nIndex];
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr[nIndex];
    siStartInfo.hStdInput = g_hChildStd_IN_Rd[nIndex];
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process. 

    bSuccess = CreateProcess(NULL,
        szCmdline,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION 

     // If an error occurs, exit the application. 
    if (!bSuccess)
        ErrorExit((PTSTR)TEXT("CreateProcess"));
    else
    {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example. 

        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);

        // Close handles to the stdin and stdout pipes no longer needed by the child process.
        // If they are not explicitly closed, there is no way to recognize that the child process has ended.

        CloseHandle(g_hChildStd_OUT_Wr[nIndex]);
        CloseHandle(g_hChildStd_IN_Rd[nIndex]);
    }

}

void WriteToPipe(int nIndex)

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;

    for (;;)
    {
        bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) break;

        bSuccess = WriteFile(g_hChildStd_IN_Wr[nIndex], chBuf, dwRead, &dwWritten, NULL);
        if (!bSuccess) break;
    }

    // Close the pipe handle so the child process stops reading. 

    if (!CloseHandle(g_hChildStd_IN_Wr[nIndex]))
        ErrorExit((PTSTR)TEXT("StdInWr CloseHandle"));
}

void ReadFromPipe(int nIndex)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;
    HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    for (;;)
    {
        bSuccess = ReadFile(g_hChildStd_OUT_Rd[nIndex], chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) break;

        bSuccess = WriteFile(hParentStdOut, chBuf,
            dwRead, &dwWritten, NULL);
        if (!bSuccess) break;
    }
}

void ErrorExit(PTSTR lpszFunction)

// Format a readable error message, display a message box, 
// and exit from the application.
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}

bool CreateMemoryMapping()
{
    m_hMapping = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        256,                    // maximum object size (low-order DWORD)
        szName);                 // name of mapping object

    if (m_hMapping == NULL)
    {
        _tprintf(TEXT("Could not create file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }

    //检查有效性
    if (INVALID_HANDLE_VALUE == m_hMapping)
    {
        std::cout << "initial file mapping Handel failed," << std::endl;
        CloseHandle(m_hMapping);
        CloseHandle(m_hFile);
        return false;
    }
    //将文件映射到进程的地址空间中	
    if (!(m_pBase = MapViewOfFile(m_hMapping, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, NULL)))
    {
        std::cout << "将文件映射到进程的地址空间中 failed," << std::endl;
        CloseHandle(m_hMapping);
        CloseHandle(m_hFile);
        return false;
    }

    m_pBase = (LPTSTR)MapViewOfFile(m_hMapping, FILE_MAP_WRITE, 0, 0, 0);
    if (!m_pBase)
    {
        std::cout << "映射文件读取 failed," << std::endl;
        CloseHandle(m_hMapping);
        return false;
    }
    LPTSTR str = (PTSTR)TEXT("hello client");
    lstrcpy((LPTSTR)m_pBase, str);

    return true;
}

bool ClearMapping()
{
    if (!UnmapViewOfFile(m_pBase))
    {
        std::cout << "unmapping failed" << std::endl;
    }
    CloseHandle(m_hMapping);
    CloseHandle(m_hFile);
    m_pBase = nullptr;

    return true;
}
