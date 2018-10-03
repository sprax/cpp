// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN64
#include <tchar.h>
#else
#include <wchar.h>
#define TCHAR   char
#define _TCHAR wchar_t
#define _tprintf wprintf
#endif


// TODO: reference additional headers your program requires here
