#include <windows.h>
#include <process.h>
#include <iostream>

//#include <boost/thread.hpp>

using namespace std;

void Func1(void *);
void Func2(void *);

CRITICAL_SECTION Section;

int threadTest()
{

    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    DWORD numCPU = sysinfo.dwNumberOfProcessors;
    printf("GetSystemInfo found %lu processors\n", numCPU);

    SYSTEM_LOGICAL_PROCESSOR_INFORMATION sysLogProcInfo;
    DWORD  returnLength = sizeof(sysLogProcInfo);

    BOOL found = GetLogicalProcessorInformation(&sysLogProcInfo, &returnLength);

    HANDLE hThreads[2];

    InitializeCriticalSection(&Section);

    hThreads[0] = (HANDLE)_beginthread(Func1, 0, NULL);
    hThreads[1] = (HANDLE)_beginthread(Func2, 0, NULL);

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    DeleteCriticalSection(&Section);

    cout << "Main exit" << endl;

    return 0;
}

void Func1(void *P)
{
    int Count;

    for (Count = 1; Count < 11; Count++)
    {
        EnterCriticalSection(&Section);
        cout << "Func1 loop " << Count << endl;
        LeaveCriticalSection(&Section);
    }
    return;
}

void Func2(void *P)
{
    int Count;

    for (Count = 10; Count > 0; Count--)
    {
        EnterCriticalSection(&Section);
        cout << "Func2 loop " << Count << endl;
        LeaveCriticalSection(&Section);
    }
    return;
}
