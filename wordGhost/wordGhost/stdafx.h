// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

//#pragma warning( disable : 4100 )   // unreferenced formal parameter
#pragma warning( disable : 4512 )   // assignment operator could not be generated

#define WIN32_LEAN_AND_MEAN	// Exclude bells and whistles from Windows headers (e.g. Media Center Extensions)
#define VC_EXTRALEAN		// Exclude even more stuff from Windows headers (e.g. COMM, RPC, sound)
#include <windows.h>        // For Windows-specific threading and synchronization functions
#include <signal.h>
#include <typeinfo>

#include <map>
#include <hash_map>
#include <hash_set>
#include <set>

#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
#include <assert.h>
#else
#define NDEBUG          1
#define assert(_expr_)  ((void)0)
#endif




//////////////////////////////////////////////////////////////////////////////

#include "spraxUtil.hpp"
#include "WordMap.hpp"

