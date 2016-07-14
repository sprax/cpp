// TrieNode.cpp : trie nodes for words
// Sprax Lines,  July 2010

#include "stdafx.h"

#include "spraxUtil.h"

#define WIN32_LEAN_AND_MEAN	// Exclude bells and whistles from Windows headers (e.g. Media Center Extensions)
#define VC_EXTRALEAN		// Exclude even more stuff from Windows headers (e.g. COMM, RPC, sound)
#include <windows.h>        // For Windows-specific threading and synchronization functions
#include <signal.h>

#include <random>

#if TIME_IS_MILLISECONDS        // defined in stdafx.h
extern TimeT  GetTime( void ) {  return timeGetTime(); } // milliseconds
extern int    DiffTimeSeconds(TimeT later, TimeT earlier) { return (later - earlier)/1000; }
extern double DeltaTimeSeconds(TimeT later, TimeT earlier) { return (later - earlier)/1000.0; }
#else
extern TimeT  GetTime( void ) {  return time(0); } // seconds since 1969
extern int    DiffTimeSeconds(TimeT later, TimeT earlier) { return (later - earlier); }
extern double DeltaTimeSeconds(TimeT later, TimeT earlier) { return (double)(later - earlier); }
#endif

bool randBool()
{
    return 1 == rand() % 2;
}

int randInRange( int rangeMin, int rangeMax )
{
    // Return a random number in the half-closed interval [rangeMin, rangeMax).
    // That is, rangeMin <= random number < rangeMax.
    // Note that RAND_MAX 0x7fff is smallish.

    return (int)(rangeMin +  (double)(rangeMax - rangeMin) * rand() / (RAND_MAX + 1));
}


bool getMinAndMax(const int A[], int len, int& minVal, int& maxVal)
{
    if (A == NULL || len < 1)
        return false;

    minVal = A[0];
    maxVal = minVal;
    if (len < 2)
        return true;
    
    for (int j = 1; j < len; j++) {
        if (minVal > A[j])
            minVal = A[j];
        else if (maxVal < A[j])
            maxVal = A[j];
    }
    return true;
}


int test_spraxUtil( void )
{
   // Seed the random-number generator with the current time so that
   // the numbers will be different every time we run.
   srand( GetTime() );

   for (int j = 0; j < 17; j++) {
        printf("randBool: %s\n", (randBool() ? "true" : "false") );
   }
   printf("\n");

   for (int j = 0; j < 17; j++) {
        printf("randInRange: %d\n", randInRange( -100, 100) );
   }
   printf("\n");

   return 0;
}

