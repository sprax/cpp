// WordRectSearchMgr.hpp : find largest dense rectangles of words from a dictionary, multi-threaded
// Sprax Lines, July 2010

#ifndef WordRectSearchMgr_hpp
#define WordRectSearchMgr_hpp

#include "WordRectFinder.hpp"

class WordRectSearchMgr
{
public:
    static const int  gMaxFinders = 16;     // hard max number of worker threads -- used to declare array

    static WordRectSearchMgr& getInstance() {
        if(!sInstance ) {
            sInstance = new WordRectSearchMgr();
        }
        return *sInstance;
    }

    void setOptions(bool findSmallerFirst, bool verbose, bool useMaps);

    void searchForLargestWordRect(const char *dictionaryFileName, int minArea, int minTall, int maxTall, int maxArea);

private:
    WordRectSearchMgr() : mWordMaps(NULL), mAscending(false), mUseMaps(false) {} // private default constructor
    WordRectSearchMgr(const WordRectSearchMgr&);                // don't define
    WordRectSearchMgr& operator=(const WordRectSearchMgr&);     // don't define
   ~WordRectSearchMgr() {}                                      // don't delete anything

    int  initFromSortedDictionaryFile(WordTrie *mTries[], WordMap *maps, const char *fname);    // loads only once

    static void sigintHandler ( int sig );
    static unsigned long finderThreadFunc( void *pvArgs );
    static int nextWantWideTall(int& wide, int& tall, const int& minTall, const int& maxTall
        , const int& minArea, const int& maxArea, const int& maxWide, bool ascending);
   
    WordTrie  ** mTries;                        // pointer to array of tries, indexed by word length
    WordMap    * mWordMaps;                     // pointer to array of word maps, indexed by word length
    
    // member data
    bool                          mAscending;   // search in order (roughly) of ascending area (default value is false)
    bool                          mUseMaps;     // use std::maps upper_bound to find next row word candidate (slower)
 
    // static data
    struct FinderThreadInfo_tag;                          // forward declaration
    static FinderThreadInfo_tag * sFinders[gMaxFinders];  // array for tracking workers
    static int                    sNumFinders;            // current number of worker threads
    static int                    sMaxFinders;            // soft maximum, as limited to twice the number of processors
    static WordRectSearchMgr    * sInstance;
    static bool                   sVerbosity;             // Only on or off for now.  False means suppress thread begin/end messages.
};

#endif // WordRectSearchMgr_hpp