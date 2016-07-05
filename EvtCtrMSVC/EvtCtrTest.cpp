// try.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EvtCtr.h"


//////////////////////////////////////////////////////////////////////////////

#define TEST_DbgStdErrToStdOut    1

static UINT32 TestFlags = 0;

void    printError( char *fmt, ... )
{
    char errs[256];
    va_list args ;
    va_start( args, fmt );
    if (TestFlags & TEST_DbgStdErrToStdOut)
        vfprintf( stdout, fmt, args );
    else
        vfprintf( stderr, fmt, args );
    vsprintf_s( errs, fmt, args );
    va_end( args );
#ifdef WIN32
    OutputDebugString( errs );
#endif
}


void printStatus( char *fmt, ... )
{
    char errs[256];
    va_list args ;
    va_start( args, fmt );
    vfprintf( stdout, fmt, args );
    vsprintf_s( errs, fmt, args );
    va_end( args );
#ifdef WIN32
    OutputDebugString( errs );
#endif
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
static  double HpcFreqDouble;
static  unsigned long loopDelay = 1;
static  unsigned long delayCounts = 0;

static  void    initCurrentTime()
{
#define MICRO_SECOND 1000000.0

  LARGE_INTEGER freqLargeInt ;

  QueryPerformanceFrequency( &freqLargeInt );

  HpcFreqDouble = freqLargeInt.HighPart * 4*1024.*1024.*1024. + freqLargeInt.LowPart ;
  delayCounts = (unsigned long)(loopDelay * (freqLargeInt.QuadPart / MICRO_SECOND));
  printf( "delayCounts: %ld\n", delayCounts );
}




/*****************************************************************************
 * Gets the performance counter and converts it to seconds.
 ****************************************************************************/
double  getCurrentTime()
{
    LARGE_INTEGER   timeLargeInt;
    double  time ;

    QueryPerformanceCounter( &timeLargeInt );

    time = timeLargeInt.HighPart * (1024.0*1024.0*1024.0)*4 + timeLargeInt.LowPart ;
    time = time / HpcFreqDouble;

    return time;
}


/*****************************************************************************
 * Busy/waits at least the requested number of microseconds.
 ****************************************************************************/
void microWait()
{
  LARGE_INTEGER start, now;

  QueryPerformanceCounter( &start );

  do {
    QueryPerformanceCounter( &now );
  } while (now.QuadPart - start.QuadPart < delayCounts);
}


#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif /* M_PI */

static const unsigned int NUM_EVT  = 555;
static const unsigned int MAX_ITER = 5;


int main(int argc, char* argv[])
{
    unsigned int j, k, n, numIter = MAX_ITER;
    double  dd;
	time_t tT;
    TimeT  tZ, tA = EvtCtr::GetTime();
    EvtInt *pEvtInt[NUM_EVT*MAX_ITER];
    EvtDbl *pEvtDbl[NUM_EVT*MAX_ITER];
    EvtCtrInt   evtCtrInt("EvtCtrInt",   tA, 0, 3 );
    EvtCtrFlt   evtCtrFlt("EvtCtrFlt",   tA, 0, 4 );
    EvtCtrDbl  *pEvtCtrDbl = new EvtCtrDbl("EvtCtrDbl",   tA, 33, 6 );
    EvtCtrBool  evtCtrBool("EvtCtrBool", tA, 0, 6 );
    SizeT idxLastAdd;

    srand( (unsigned int) time(&tT) );

printStatus(" _MSC_VER is (%d)\n",  _MSC_VER );
//exit(0);

// The list of event counters
    // 3rd parm == N means we're testing the realloc on every Nth add
    EvtCtrMgr  listEvtCtr(tA, 40, 1); // usual args: just (tA)

    if (argc > 1) {
        j = atoi(argv[1]);
        numIter = j > 1 ? j : 1;
        if (numIter > MAX_ITER)
            numIter = MAX_ITER;
    }

    printError("SIZES: Bool(%u)  EvtInt(%u)  EvtDbl(%u)  EvtCtrInt(%u)  EvtCtrMgr(%u)\n"
              , sizeof(Bool), sizeof(EvtInt), sizeof(EvtDbl), sizeof(EvtCtrInt), sizeof(EvtCtrMgr));

    initCurrentTime();
    double d0 = getCurrentTime();

    for (j = n = 0; j < 22; j++) {
        k = rand() % 1000;
        Sleep(k);
        dd = getCurrentTime();
        printError( "j%4d  k%4d dd:%12.3f  df:%6.3f\n", j, k, dd, dd - d0 );
        if (++n == 10 || dd - d0 > 3.0) {
            printError( "triggered: n=%4d  df=%6.3f\n", n, dd - d0 );
            n  = 0;
            d0 = dd;
        }
    }

    TimeT tB = tA;
    for (unsigned int iter = 1; iter <= numIter; iter++) {
        printError("%s: iteration %d at time %u...................\n", argv[0], iter, tB);

        idxLastAdd = listEvtCtr.Add(&evtCtrInt); // add this same ctr every iteration (just testing)
        for (j = 0; j < iter*NUM_EVT; j++) {
            pEvtInt[j] = new EvtInt(tA + j*10, j);
            pEvtDbl[j] = new EvtDbl(tA + j*11, j*M_PI);
            idxLastAdd =   evtCtrInt.Add( *pEvtInt[j] );
            idxLastAdd = pEvtCtrDbl->Add( *pEvtDbl[j] );
        }
        idxLastAdd = listEvtCtr.Add(pEvtCtrDbl);

        Sleep(100);             // sleep 0.100 seconds
        tZ = evtCtrInt.GetTime();
        dd = evtCtrInt.GetMaxRate();
        evtCtrInt.UpdateMinMaxAndRate(tZ);
        dd = evtCtrInt.GetMaxRate();

        evtCtrInt.PrintCsvHead();
        evtCtrInt.PrintCsvData();

        listEvtCtr.ComputeRatesNow();
        listEvtCtr.PrintCsvAll();

        listEvtCtr.ResetAll();
        for (j = 0; j < iter*NUM_EVT/2; j++) {
            pEvtInt[j] = (delete pEvtInt[j], new EvtInt(tZ + j*100, j));
            pEvtDbl[j] = (delete pEvtDbl[j], new EvtDbl(tZ + j*110, j*M_PI));
            idxLastAdd = evtCtrInt.Add( *pEvtInt[j] );
            idxLastAdd = pEvtCtrDbl->Add( *pEvtDbl[j] );
        }

        evtCtrInt.PrintCsvData();
        
        Sleep(10); // sleep 1/100 second
        tB = EvtCtr::GetTime();
        listEvtCtr.ResetAll(tB);
    }
    
    // Clean-up memory
    for (j = 0; j < numIter*NUM_EVT; j++) {
        delete pEvtInt[j];
        delete pEvtDbl[j];
    }
    delete pEvtCtrDbl;
	return 0;
}

