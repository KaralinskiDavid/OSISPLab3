#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <iostream>

void _tmain(int argc, TCHAR* argv[])
{
    CRITICAL_SECTION criticalsection;
    InitializeCriticalSection(&criticalsection);
    TCHAR szName[] = TEXT("MyFileMappingObject");
    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CRITICAL_SECTION), szName);                

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not create file mapping object (%d).\n"),
            GetLastError());
        return;
    }

    pBuf = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(criticalsection));

    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hMapFile);

        return;
    }

    SIZE_T datasize = sizeof(CRITICAL_SECTION);

    CopyMemory((PVOID)pBuf, &criticalsection, datasize);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    STARTUPINFO si1;
    PROCESS_INFORMATION pi1;

    STARTUPINFO si2;
    PROCESS_INFORMATION pi2;

    std::wstring CommandLineFirst(L"D:\\Work\\OSiSP5\\Lab_3\\Debug\\FirstProcess.exe");
    std::wstring CommandLineSecond(L"D:\\Work\\OSiSP5\\Lab_3\\Debug\\SecondProcess.exe");
    std::wstring CommandLineThird(L"D:\\Work\\OSiSP5\\Lab_3\\Debug\\ThirdProcess.exe");

    LPWSTR lpwCmdLineFirst = &CommandLineFirst[0];
    LPWSTR lpwCmdLineSecond = &CommandLineSecond[0];
    LPWSTR lpwCmdLineThird = &CommandLineThird[0];

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);
    ZeroMemory(&pi1, sizeof(pi1));

    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi2, sizeof(pi2));

    if (!CreateProcess(NULL, lpwCmdLineFirst, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    if (!CreateProcess(NULL, lpwCmdLineSecond, NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    if (!CreateProcess(NULL, lpwCmdLineThird, NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    UnmapViewOfFile(&criticalsection);
    CloseHandle(hMapFile);

    DeleteCriticalSection(&criticalsection);
}