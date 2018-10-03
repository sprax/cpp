// histarea.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// default values:
static const char *defProgramName  = "histarea";

static void usage(int argc, _TCHAR* argv[], const TCHAR* programName, const _TCHAR *reason=NULL)
{
    _tprintf( L"Usage: %s [-o]  blah blah\n", defProgramName);

    _tprintf( L"Exiting from this command:\n    ");
    for (int j = 0; j < argc; j++) {
        _tprintf( L"%s ", argv[j]);
    }
    printf("\n");
    if (reason) {
        _tprintf(L"Reason: %s\n", reason);
    }
    exit(0);
}

int histarea(int histogram[], int length)
{
    int area = 0;
    if (length > 2) {
        int *maxFromLeft  = (int *)malloc(length * sizeof(int));
        int  height, maxHeight = INT_MIN;
        for (int j = 0; j < length; j++) {  // Don't skip the last entry.
            height = histogram[j];
            if (maxHeight < height)
                maxHeight = height;
            maxFromLeft[j] = maxHeight;
        }
        maxHeight = INT_MIN;
        for (int j = length; --j > 0; ) {   // Do skip the first entry.
            height = histogram[j];
            if (maxHeight < height)
                maxHeight = height;
            if (maxHeight > maxFromLeft[j])
                area += maxFromLeft[j] - height;
            else
                area += maxHeight - height;
        }
        free(maxFromLeft);
    }
    return area;
}

int test_histarea(int histogram[], int length)
{
	int area = histarea(histogram, length);
	_tprintf(L"Got area %d from histogram: \n", area);
	for (int j = 0; j < length; j++)
		_tprintf(L"%d  ", histogram[j]);
	_tprintf(L"\n");
	return area;
}

#ifdef _WIN64
    int _tmain(int argc, _TCHAR* argv[])
#else
    int main(int argc, char* argv[])
#endif
{
    const char *programName  = argv[0] ? argv[0] : defProgramName;
    //usage(argc, argv, programName, L"This program is a stub.");

	int histoA[] = { -1, 34, 3, -14, 4, 0, -8, 17, 0 };
	int length = sizeof(histoA) / sizeof(int);
	test_histarea(histoA, length);

	int histoB[] = { -1, -2, -3, -4, -5, -4, -3, -2, -1 };
	length = sizeof(histoB) / sizeof(int);
	test_histarea(histoB, length);

	int histoC[] = { 0, 1, 2, 3, 4, 5, 4, 3, 2, 1 };
	length = sizeof(histoC) / sizeof(int);
	test_histarea(histoC, length);

	return 0;
}
