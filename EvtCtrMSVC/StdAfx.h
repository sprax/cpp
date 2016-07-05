// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AE554A40_4B15_4DBD_ADAD_C1F1C2E2378B__INCLUDED_)
#define AFX_STDAFX_H__AE554A40_4B15_4DBD_ADAD_C1F1C2E2378B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include <mmsystem.h>  // for timeGetTime
#include <time.h>

// #include <string>    //  No need for a real string class 

void printError(char *fmt, ...);

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AE554A40_4B15_4DBD_ADAD_C1F1C2E2378B__INCLUDED_)
