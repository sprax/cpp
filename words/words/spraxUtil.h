// spraxUtil.hpp : common functions
// Sprax Lines,  July 2010

#ifndef spraxUtil_hpp
#define spraxUtil_hpp

#ifndef WIN32
#include <stdio.h>
#include <map>
#ifdef _DEBUG
#include <assert.h>
#else
#define NDEBUG          1
#ifndef assert
#define assert(_expr_)  ((void)0)
#endif
#endif
#endif


// typedef SizeT & TimeT: ////////////////////////////////////////////////////

typedef size_t          SizeT;      // an unsigned type
typedef unsigned short  WordLenT;   // word length type (unsigned)
#define WORDLEN_MAX     USHRT_MAX   // max word length == max val for this unsigned type

#define TIME_IS_MILLISECONDS    1
#ifdef  TIME_IS_MILLISECONDS

#ifdef WIN32
#include <mmsystem.h>               // for timeGetTime
typedef   DWORD  TimeT;             // wrap-around is possible, type must be unsigned
extern TimeT  GetTime( void );      // milliseconds since system boot
#endif


#else

#define TIME_IS_UNIX_SECONDS
#include <time.h>
typedef   time_t TimeT;             // wrap-around not until 2038, so use built-in type (long)
extern TimeT  GetTime( void );      // seconds since 1969

#endif

extern int      DiffTimeSeconds(TimeT later, TimeT earlier);
extern double   DeltaTimeSeconds(TimeT later, TimeT earlier);


bool    randBool();
void    randInRange( int rangeMin, int rangeMax, int num );
bool    getMinAndMax(const int A[], int len, int& minVal, int& maxVal);
int     test_spraxUtil( void );

#endif // spraxUtil_hpp
