4// wordRectSearchMgr.cpp : find largest dense rectangles of words from a dictionary, multi-threaded
// Sprax Lines, July 2010
//
// TODO: 
//      Save memory by not loading dictionary words too long or short for the specified search.
//      If adding more search methods, turn wordRectFinder into an interface and let a factory make the 
//          concrete subclasses.
//      Limit wordRows & colNodes to actual dimensions?    
//          Pros: *slightly* smaller footprint;
//          Cons: don't expect much speed-up (reducing L2 cache misses?); calling new and delete 
//              could introduce bottlenecks
//      Maybe divide searchForLargestWordRect into separate init, search, and quit methods, and
//          allow multiple instances with init and teardown done only once?  No, too much work,
//          no real pay-off.  As of now, the methods here can call exit on any error condition.
//      Send error messages to stderr instead of stdout?
//      Convert printf's to cout or something else?  Right now, the i/o buffering sees to play well 
//          among the threads (maybe just because of the critical section).


#include "stdafx.h"
#include <time.h>           // ctime_s

#include "WordRectSearchMgr.hpp"

WordRectSearchMgr::FinderThreadInfo_tag * WordRectSearchMgr::sFinders[gMaxFinders];
int                         WordRectSearchMgr::sNumFinders = 0;
int                         WordRectSearchMgr::sMaxFinders = 0;
bool                        WordRectSearchMgr::sVerbosity  = false;
WordRectSearchMgr         * WordRectSearchMgr::sInstance;

// Synchronization objects to be shared by the worker threads, their manager, and the sigint handler object:
// (Declared at file scope to keep windows.h out of WordRectMgr.hpp.  Make them member vars if converting to DLL.)
static HANDLE               fsMaxThreadsSemaphore;
static HANDLE               fsThreadStartedEvent;
static CRITICAL_SECTION     fsFindersCriticalSection;

typedef struct WordRectSearchMgr::FinderThreadInfo_tag {      // private struct type for tracking finder threads
    WordRectFinder * const  mpWRF;
    volatile int            mFinderIdx;
    HANDLE                  mThreadHandle;

    FinderThreadInfo_tag(WordRectFinder *wrf) : mpWRF(wrf), mFinderIdx(-1), mThreadHandle(INVALID_HANDLE_VALUE)
    {};
   ~FinderThreadInfo_tag() { delete mpWRF; }    // FinderThreadInfo takes ownership of its initializng WordRectFinder
}   FinderThreadInfo;


int WordRectSearchMgr::initFromSortedDictionaryFile(WordTrie *tries[], WordMap wordMaps[], const char *fname) // TODO: switch to the generic in Trie.hpp
{   
    // This method allocates memory for holding all the words, and that memory is retained 
    // until the process terminates.  So it should succeed only once.
    static bool alreadyLoaded = false;
    if (alreadyLoaded) {
        return 0;
    }
    alreadyLoaded = true;

    FILE *fi;
    if ( ! fopen_s(&fi, fname, "r") ) {
        printf("Opened %s\n" , fname);
    } else {
        printf("Error opening dictionary file: %s\n", fname);
        return 0;
    }

    WordNode *prevNode[BUFSIZE] = { NULL, };
    char word[BUFSIZE];
    int numWords = 0, maxWordLen = 0;
    while (fgets(word, BUFSIZE, fi))  {
        // got next word as a line in the file; now get its length, and remove any trailing newline.
        WordLenT length = (WordLenT)strlen(word);
        if (word[length - 1] < WordNode::sFirstLetter) {
            word[length - 1] = NULL;
            length = length - 1;
        }
        if (maxWordLen < length)  {
            maxWordLen = length;
        }
        char *saveWord = _strdup(word);		// Allocate word's memory here, for the life of the program.
        numWords++;
        prevNode[length] = tries[length]->addWord(saveWord, length, prevNode[length]);
        if (wordMaps != NULL) {
            assert(length > 0);
            assert(length <= MAXLEN);
            wordMaps[length].insert(WordMap::value_type(saveWord, numWords));   // strictly increasing value preserves the order
        }
    }
    fclose(fi);
    return maxWordLen;
}

void WordRectSearchMgr::setOptions(bool findSmallerFirst, bool verbosity, bool useMaps)
{
    mAscending = findSmallerFirst;
    sVerbosity = verbosity;             // only on or off, for now
    mUseMaps   = useMaps;
}


void WordRectSearchMgr::searchForLargestWordRect(const char *dictionaryFileName, int minArea, int minTall, int maxTall, int maxArea)
{   
    WordTrie * triesMem[MAXLEN];                                // The actual memory for the mTries is here on the stack
    mTries   = &triesMem[-1];                                   // index mTries by word length (not by strlen(word) - 1); mTries[0] will bomb. 
    for (int wordLen = 1; wordLen <= MAXLEN; wordLen++) {
        mTries[wordLen] = new WordTrie(wordLen);                // create (and later delete) them in a loop; we don't want a default constructor
    }

    WordMap *wordMaps_mem = NULL;
    if (mUseMaps) {
        wordMaps_mem = new WordMap[MAXLEN];                      // delete wordMaps_mem when all searching is done
        mWordMaps    = &wordMaps_mem[-1];                        // index wordMaps by word length (not length-1); using maps[0] would bomb.
    }

    // Load from the dicionary only once; word/trie/map memory is retained for life of the program.
    int maxWordLen = initFromSortedDictionaryFile(mTries, mWordMaps, dictionaryFileName);
    if (maxWordLen == 0) {
        printf("Found no words in dictionary %s; exit(1)\n", dictionaryFileName);
        exit(1);
    }
    if (maxWordLen > MAXLEN) {
        printf("Found word(s) of length > %d in dictionary %s; exit(2)\n", dictionaryFileName, MAXLEN);
        exit(2);
    }

    // Init the number of word rect finder threads:
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    int numProcessors = (int)sysinfo.dwNumberOfProcessors;
    int idealNumThreads = 3*numProcessors/2;                // TODO: number is somewhat empirical, somewhat arbitrary
    sMaxFinders = idealNumThreads < gMaxFinders ? idealNumThreads : gMaxFinders;
    printf("Found %lu processors (or cores); will search with up to %d threads.\n", numProcessors, sMaxFinders);

    fsMaxThreadsSemaphore = CreateSemaphore(NULL, (LONG)sMaxFinders, (LONG)sMaxFinders, NULL);
    if (fsMaxThreadsSemaphore == NULL) 
    {
        printf("CreateSemaphore error: %d\n", GetLastError());
        return;
    }
    fsThreadStartedEvent  = CreateEvent(NULL, true, false, NULL);
    InitializeCriticalSection(&fsFindersCriticalSection);

    TimeT   timeBeg = GetTime();
    char    ctimeBuf[ BUFSIZE ];
    time_t  unixTimeBeg = time(0);
    ctime_s(ctimeBuf, BUFSIZE, &unixTimeBeg );
    printf("Starting the search at %s\n", ctimeBuf);
    signal( SIGINT, sigintHandler );
    printf("    Hit Ctrl-C to see the word rectangle(s) in progress...\n\n");

    WordRectFinder::setMinimumArea(minArea);
    WordRectFinder::setCollectiveStartTime(timeBeg);

    DWORD result, threadId;
    int  wantWide = -1, wantTall = -1;
    while( nextWantWideTall(wantWide, wantTall, minTall, maxTall, minArea, maxArea, MAXLEN, mAscending) ) {
 
        int wantArea = wantWide*wantTall;
        int haveArea = WordRectFinder::getFoundArea();
        if (wantArea <= haveArea) {
            printf("Skip   %2d * %2d because %3d <= %3d (area already found)\n", wantWide, wantTall, wantArea, haveArea);
            continue;
        }

        // Create a new worker thread.  We don't try to control or even observe the 
        // worker threads here in this loop.  Just create one and let it synchronize itself
        // with any that are already running.  If the max number of worker threads are already
        // running, then before this new one actually starts searching, it will have to wait 
        // (on a semaphore) until one of those other threads exits.  The only synchronization 
        // here in this loop is to wait (on an event) for each new thread to start running before 
        // creating another one.  The  threadFunc will be responsible for deleting the WordRectFinder 
        // that we create here and pass in to CreateThread.  (That's easier than making a WRF-factory
        // just for the thread func!)

        WordRectFinder   *pWRF = new WordRectFinder(*mTries[wantWide], *mTries[wantTall], mUseMaps ? mWordMaps : NULL);
        HANDLE threadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)finderThreadFunc, pWRF, 0, &threadId );
        if (threadHandle == INVALID_HANDLE_VALUE) {
            delete pWRF;
            printf("\n    CreateThread failed for %d x %d finder!  Calling exit(2).\n", wantWide, wantTall);
            exit(2);
        }
        pWRF = NULL;    // Forget we ever knew (or newed) this pointer!   ;p

        // We don't strictly need to wait for this thread to start before initializing the next
        // one in the loop, because we are not initializing these threads using pointers to 
        // local memory that might change between the time the thread is created and the time
        // it actually starts processing its function arguments.  (Just calling Sleep(1) would
        // *probably* prevent such slippage.  But it's nice to make sure the threads start in 
        // the order they are created here, and not to create a whole lot of them before we know
        // they are needed, or that will just get aborted anyway.  
        // So we wait for the child thread to set this event, then reset it:
        result = WaitForSingleObject(fsThreadStartedEvent, INFINITE);      // 
        if (result == WAIT_FAILED) {
            printf("WaitForSingleObject failed on the start-one-finder-at-a-time event; error: %d  continuing\n", GetLastError());
        }
        (void)ResetEvent(fsThreadStartedEvent);                         // ignore the return value
    }

    // Wait for all remaining finder threads to end.  If the sigint handler happens to be active,
    // these finder threads must wait for it to yield the critical section before they can die.
    HANDLE finderThreadHandles[gMaxFinders];
    EnterCriticalSection(&fsFindersCriticalSection);
    for (int j = 0; j < sNumFinders; j++) {
        finderThreadHandles[j] = sFinders[j]->mThreadHandle;
    }
    LeaveCriticalSection(&fsFindersCriticalSection);
    WaitForMultipleObjects(sNumFinders, finderThreadHandles, true, INFINITE);

    for (int wordLen = 1; wordLen <= MAXLEN; wordLen++) {
        delete mTries[wordLen];
    }
    if (wordMaps_mem != NULL) {
        delete [] wordMaps_mem;
    }

    time_t unixTimeEnd = time(0);
    ctime_s(ctimeBuf, BUFSIZE, &unixTimeEnd );
    printf("\nFinished the search at %s", ctimeBuf);
    printf("Elapsed time: %d seconds\n", (int)(unixTimeEnd - unixTimeBeg) );

    CloseHandle(fsMaxThreadsSemaphore);
    CloseHandle(fsThreadStartedEvent);
    DeleteCriticalSection(&fsFindersCriticalSection);
    return;
}


unsigned long WordRectSearchMgr::finderThreadFunc( void *pvArgs )
{
    WordRectFinder   * pWRF = static_cast<WordRectFinder *>(pvArgs);
    int   wantWide  =  pWRF->getWide();
    int   wantTall  =  pWRF->getTall();
    TimeT  timeZero =  pWRF->getCollectiveStartTime();

    // Use the passed-in pWRF to construct our FinderThreadInfo on the thread-local stack.
    // When this FTI falls out of scope, its destructor will delete the pWRF.
    // That's it for memory managment here.  Unless the synchronization or duplicat handle
    // calls are doing it, there heap memory is allocated by this func or the WRF methods it calls.
    FinderThreadInfo myFTI(pWRF);

    DWORD result  = WaitForSingleObject(fsMaxThreadsSemaphore, INFINITE);
    if (result != WAIT_OBJECT_0) { 
        printf("Error waiting for MaxThreadsSemaphore: res:%d  err:%d;  exit(3)\n", result, GetLastError() );
        exit(3);
    }

    // The pseudo handle returned by GetCurrentThread is no good for synchronization by another
    // thread.  To allow our main thread to wait on this thread, we need to give it a handle
    // that has synchronization access rights.
    HANDLE hPseudo = GetCurrentThread();
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hRealThread = 0;
    BOOL success = DuplicateHandle(hProcess, hPseudo, hProcess, &hRealThread, SYNCHRONIZE, false, 0);
    if (!success) {
        printf("DuplicateHandle failed with error: %d  exit(4)\n", GetLastError() );
        exit(4);
    }
    myFTI.mThreadHandle = hRealThread;

    // Synchronize checking on the current largest word rect + writing to shared storage (sFinders):
    EnterCriticalSection(&fsFindersCriticalSection);
    TimeT  timeNow = GetTime();

    // Now that we are past the semaphore and inside the critical section, we check again
    // whether this finder is still needed, or has been scooped by another while we waited to start.  
    const int wantArea     =  wantWide*wantTall;
    const int begFoundArea =  pWRF->getFoundArea();
    const bool doRegister  = (wantArea > begFoundArea);

    if (doRegister) {
        int minimumArea  = pWRF->getMinimumArea();
        char equalsChar  = '=';         // if we have not yet found a wordRect, we want area >= minimum
        if (minimumArea <= begFoundArea) {
            minimumArea  = begFoundArea;
            equalsChar   = ' ';         // if we have already found a wordRect, we want area >  found
        }
        if (sVerbosity) {
            printf("TRY    %2d * %2d because %3d >%c %3d (thread %2d began after %2d seconds)\n"
                , wantWide, wantTall, wantArea, equalsChar, minimumArea
                , sNumFinders, DiffTimeSeconds(timeNow, timeZero));
        }
        // We're about to put the address of our local FTI into shared storage (sFinders), 
        // to be used by the manager's wait functions and sigint handler.   Obviously that 
        // handler had better lock and make sure this thread is still alive before dereferencing
        // the pointer, and likewise, we have to lock and remove it from that storage before deleting it below.
        myFTI.mFinderIdx = sNumFinders;
        assert(sNumFinders < sMaxFinders);
        assert(sFinders[sNumFinders] == NULL);
        sFinders[sNumFinders] = &myFTI;    // Add our info to end slot, which should already be empty.
        ++sNumFinders;
    } else {
        if (sVerbosity) {
            printf("ABORT  %2d * %2d because %3d <= %3d (thread %2d dying after %2d seconds)\n"
                , wantWide, wantTall, wantArea, begFoundArea, sNumFinders, DiffTimeSeconds(timeNow, timeZero));
        }
    }
    // We finished initializing and registering our thread info in the managers' table 
    // (if this finder was still relevant), so now we can leave the CS and set the Event.
    // Other finder threads may be waiting to begin or end, and their doing so could affect 
    // what the manager does next, so we release the CS first.  (In particular, if another 
    // thread just found the biggest wordRect, the manager may find this out by calling 
    // getFoundArea and then "decide" not start any more finder threads.)
    LeaveCriticalSection(&fsFindersCriticalSection);

    // Only then do we tell the calling loop it can resume and possibly create another finder thread:
    success = SetEvent(fsThreadStartedEvent);
    if ( ! success) {
        printf("SetEvent failed with error: %d  exit(5)\n", GetLastError() );
        exit(5);
    }

    // Before starting this search, which may take milliseconds or hours
    // (or be aborted as soon as it gets going), let's check its relevancy one more time.
    int area = -wantArea;           // Negative area means this search was aborted. 
    if (doRegister && wantArea > pWRF->getFoundArea() ) {
        area = pWRF->doSearch();
    }
    timeNow = GetTime();   // record the time this search finished or aborted, before any more waiting

    {   //beg CriticalSection block
        EnterCriticalSection(&fsFindersCriticalSection);

        // Note that if the sigint handler uses this same sync object, so it's in the middle 
        // of printing an in-progress word rect, we'll wait for it here before we check the
        // final area found, possibly print a new found rect, and then delete the pWRF.
        // by falling out of scope.
        int endFoundArea = pWRF->getFoundArea();
        if (area > 0) {
            if (endFoundArea < area) {
                pWRF->setFoundArea(area);
                pWRF->printWordRect(true, timeNow, myFTI.mFinderIdx);
            } else if (sVerbosity) {
                printf("End    %2d * %2d because %3d <= %3d (thread %2d finished in %2d seconds)\n"
                    , wantWide, wantTall, area, endFoundArea
                    , myFTI.mFinderIdx, DiffTimeSeconds(timeNow, pWRF->getStartTime()) );
            }
        } else if (area < 0) {
            if (sVerbosity) {
                int       runTime = (int)pWRF->getStartTime();
                runTime = runTime == -1 ? -1 : DiffTimeSeconds(timeNow, runTime);  // A runTime of -1 means it never started
                printf("End    %2d * %2d because %3d <= %3d (thread %2d aborted at  %2d seconds)\n"
                    , wantWide, wantTall, -area, endFoundArea
                    , myFTI.mFinderIdx, runTime );
            }
        } else { // area == 0
            if (sVerbosity) {
                printf("End    %2d * %2d because none found (thread %2d finished in %2d seconds)\n"
                    , pWRF->getWide(), pWRF->getTall()
                    , myFTI.mFinderIdx, DiffTimeSeconds(timeNow, pWRF->getStartTime()) );
            }
        }
        if (doRegister) {   // We did register, so now we must unregister.
            // Remove our FinderThreadInfo from the shared storage:
            int finderIdx = myFTI.mFinderIdx;                // this value may have changed
            assert(sFinders[finderIdx]->mpWRF == pWRF);    // assert that we are in sync
            --sNumFinders;
            assert(sNumFinders >= 0);
            sFinders[finderIdx] = sFinders[sNumFinders];    // Compactify by moving the last entry into our spot (even if it's the same).
            sFinders[finderIdx]->mFinderIdx = finderIdx;    // Tell this entry it where it now is.
            sFinders[sNumFinders] = NULL;                   // Nullify the vacated spot at the end (again, it could be our own at idx 0).
        }
        LeaveCriticalSection(&fsFindersCriticalSection);
    }   //end CriticalSection block

    // Finally, release the sempaphore (decrement the number of users)
    result = ReleaseSemaphore(fsMaxThreadsSemaphore, 1, NULL);
    if (result == 0) {
        printf("Error from ReleaseSemaphore: res:%d  err:%d  exit(6)\n", result, GetLastError() );
        exit(6);
    }
    return 0;
}


void WordRectSearchMgr::sigintHandler ( int sig )
{
    static TimeT    timeLastSigint = 0;
    static int      signalCount = 0;

    // On SIGINT (Ctrl-C), print the word rectangle(s) in progress, or, on double-Ctrl-C, exit.
    if (sig != SIGINT) {
        return;                 // handle only Ctrl-C
    }
    signal(sig,  SIG_IGN);      // ignore all Ctrl-C's until the handler is reset

    TimeT  timeNow = GetTime();
    timeLastSigint = DiffTimeSeconds(timeNow, timeLastSigint);      // seconds since last call
    if (timeLastSigint < 2) {
        printf("sigintHandler: time difference < 2 seconds -- bye bye!\n", sig, timeLastSigint);
        exit(0);
    }
    
    {
        EnterCriticalSection(&fsFindersCriticalSection);
        timeLastSigint = timeNow;                       // remember the time of this call
        for (int j = 0; j < sNumFinders; j++) {
            const FinderThreadInfo *pFTI = sFinders[j];
            if (pFTI) {
                const WordRectFinder *pWRF = pFTI->mpWRF;
                DWORD status = WaitForSingleObject(pFTI->mThreadHandle, 0);
                if (status == WAIT_TIMEOUT) {
                    // The finder thread is still alive, so display its progress
                    pWRF->printWordRect(false, timeNow, j);   // show latest (partial) word rect
                } else {
                    // The finder thread was signaled, or the wait failed
                    if (sVerbosity) {
                        printf("Thread info not found for %d x %d finder...\n", pWRF->getWide(), pWRF->getTall());
                    }
                }
            }
        }
        LeaveCriticalSection(&fsFindersCriticalSection);
    }

    if (++signalCount < 12) {                     // advise the user, the first dozen times or so...
        printf("\n    To exit, hit Ctrl-C twice, quickly.\n\n");
    } else {
        printf("\n");
    }
    signal(SIGINT, &sigintHandler);		            // reinstall this handler
}


int WordRectSearchMgr::nextWantWideTall(int& wide, int& tall, const int& minTall, const int& maxTall
    , const int& minArea, const int& maxArea, const int& maxWide, bool ascending)
{
    if (ascending) {
        if (tall < 0) {                     // special case:
            tall = wide = minTall;          // initialize
        } else if (wide < maxWide) {
            wide++;
        } else {
            tall++;
            wide = tall;
        }
        for ( ; tall <= maxTall ; ++tall, wide = tall ) {
            for ( ; wide <= maxWide ; wide++ ) {
                int area = wide*tall;
                if (minArea <= area && area <= maxArea) {
                    return area;
                }
            }
        }
    } else {
        if (wide < 0 && tall < 0) {     // special case: initialize...
            wide = maxWide;              
            tall = maxTall;             // no special checking: the case maxWide < maxTall falls through to 0
        } else if (wide > tall) {
            wide--;
        } else {
            tall--;
            wide = maxWide;
        }
        for ( ; tall >= minTall ; --tall, wide = maxWide ) {
            for ( ; wide >= tall ; wide-- ) {
                int area = wide*tall;
                if (minArea <= area && area <= maxArea) {
                    return area;
                }
            }
        }
    }
    return 0;     // default: no suitable values for wide & tall remain
}

