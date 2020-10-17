#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

TCHAR szName[] = TEXT("MyFileMappingObject");

int _tmain()
{
    HANDLE hMapFile;
    CRITICAL_SECTION* criticalsection;

    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szName);

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
            GetLastError());
        return 1;
    }

    criticalsection = (CRITICAL_SECTION*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));

    if (criticalsection == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(hMapFile);

        return 1;
    }


    bool flag = true;

    while (flag)
    {
        if (TryEnterCriticalSection(criticalsection))
        {
            for (int i = 20; i < 30; i++)
            {
                printf("%d\n", i);
                Sleep(10);
            }
            LeaveCriticalSection(criticalsection);
            flag = false;
        }
    }

    UnmapViewOfFile(criticalsection);

    CloseHandle(hMapFile);

    return 0;
}