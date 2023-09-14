#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 4096 
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");

int main(void)
{
    CHAR chBuf[BUFSIZE];
    DWORD dwRead, dwWritten;
    HANDLE hStdin, hStdout;
    BOOL bSuccess;
    HANDLE hMapFile;
    LPCTSTR pBuf;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (
        (hStdout == INVALID_HANDLE_VALUE) ||
        (hStdin == INVALID_HANDLE_VALUE)
        )
        ExitProcess(1);

    DWORD pid = GetCurrentProcessId();
    // Send something to this process's stdout using printf.
    printf("\n ** This is a message from the child process. ** [%d]\n", pid);

    // This simple algorithm uses the existence of the pipes to control execution.
    // It relies on the pipe buffers to ensure that no data is lost.
    // Larger applications would use more advanced process control.

    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        szName);               // name of mapping object

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }

    pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object
        FILE_MAP_ALL_ACCESS,  // read/write permission
        0,
        0,
        256);

    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hMapFile);

        return 1;
    }

    //MessageBox(NULL, pBuf, TEXT("Process2"), MB_OK);

    

    for (;;)
    {
        // Read from standard input and stop on error or no data.
        bSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL);

        if (!bSuccess || dwRead == 0)
            break;

        printf("[%d]%s\n", pid, chBuf);

        // Write to standard output and stop on error.
        bSuccess = WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL);

        if (!bSuccess)
            break;

        printf("[%d]%s\n", pid, pBuf);
    }
    //MessageBox(NULL, pBuf, TEXT("Process2"), MB_OK);

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    return 0;
}