// TrieNode.cpp : trie nodes for words
// Sprax Lines,  July 2010

#include "stdafx.h"
#include "spraxUtil.hpp"

#include <random>
#include <chrono>

#if TIME_IS_MILLISECONDS        // defined in stdafx.h
extern TimeT  GetTime( void ) {  return timeGetTime(); } // milliseconds
extern int    DiffTimeSeconds(TimeT later, TimeT earlier) { return (later - earlier)/1000; }
extern double DeltaTimeSeconds(TimeT later, TimeT earlier) { return (later - earlier)/1000.0; }
#else
extern TimeT  GetTime( void ) {  return time(0); } // seconds since 1969
extern int    DiffTimeSeconds(TimeT later, TimeT earlier) { return (later - earlier); }
extern double DeltaTimeSeconds(TimeT later, TimeT earlier) { return (double)(later - earlier); }
#endif

#ifndef WIN32
//TODO use this?
#include <sys/sysinfo.h>
TimeT GetTime() {
	TimeT milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
	return milliseconds_since_epoch;
}
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

