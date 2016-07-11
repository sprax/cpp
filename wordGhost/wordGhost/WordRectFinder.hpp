// wordRectFinder.hpp : find largest dense rectangles of words from a dictionary, multi-threaded
// Sprax Lines, July 2010

#ifndef WordRectFinder_hpp
#define WordRectFinder_hpp

#include "WordTrie.hpp"
#ifndef WIN32
#include <time.h>
#include "WordMap.hpp"
#endif

class WordRectFinder
{
public:

    WordRectFinder(const WordTrie& rT, const WordTrie& cT, const WordMap *maps=NULL) 
        : mRowTrie(rT), mColTrie(cT), mWantArea(rT.getWordLen()*cT.getWordLen())
        , mHaveTall(0), mStartTime((TimeT)-1), mWordMaps(maps)     // start time < 0 means never; maps used as an option
    {
        // For each column j, make colNodes[j][-1] point to the mColTrie root:
        for (int j = 0; j < BUFSIZE; j++) {
            mColNodes[j] = &mColNodesMem[j][1];
        }
        int wantWide = mRowTrie.getWordLen();
        for (int j = 0; j < wantWide; j++) {
            mColNodes[j][-1] = mColTrie.getRoot();
        }
    }
    WordRectFinder(const WordRectFinder&);       // Prevent pass-by-value by not defining copy constructor.
    WordRectFinder& operator=(const WordTrie&);  // Prevent assigment by not defining this operator.
   ~WordRectFinder() {}                          // This class points only to const shared memory.

    inline static int       getFoundArea()                  { return sFoundArea; }    
    inline static void      setFoundArea(int area)          { sFoundArea = area; } 
    inline static int       getMinimumArea()                { return sMinimumArea; }    
    inline static void      setMinimumArea(int area)        { sMinimumArea = area; } 
    inline static TimeT     getCollectiveStartTime()        { return sCollectiveStartTime; }    
    inline static void      setCollectiveStartTime(TimeT t) { sCollectiveStartTime = t; } 

    inline TimeT            getStartTime()  const   { return mStartTime;  }
    inline int              getWide()       const   { return mRowTrie.getWordLen(); }  
    inline int              getTall()       const   { return mColTrie.getWordLen(); }

    int doSearch();
    
    void printWordRect(bool complete, TimeT timeNow, int finderIdx) const;

private:

    int findWordRectRowsUsingTrieLinks(int haveTall);
    int findWordRectRowsMapUpper(int haveTall, const WordMap& rowMap, char wordCols[MAXLEN][MAXLEN]);

#ifdef         findWordRectRowsUsingGetNextWordNodeAndIndex       // obsolete, but left here as a reference implementation
    static int findWordRectRowsUsingGetNextWordNodeAndIndex(  int wantWide, int wantTall, int haveTall, WordTrie& mRowTrie, WordTrie& mColTrie
        , const char *wordRows[MAXLEN], const WordNode ** colNodes[BUFSIZE] );
#endif

    const WordTrie   &  mRowTrie;
    const WordTrie   &  mColTrie;
    const int           mWantArea;
    int                 mHaveTall;
    TimeT               mStartTime;         // Negative value means this finder has never started searching.
    const char       *  mWordRows[MAXLEN];
    const WordNode  **  mColNodes[BUFSIZE], *mColNodesMem[BUFSIZE][BUFSIZE]; // TODO: allocate these dynamically, to dims [wantWide][wantTall+1]?
    const WordMap    *  mWordMaps;          // used only by findWordRectRowsMapUpper; to add more finders, consider using templates or a factory

    static int          sFoundArea;
    static int          sMinimumArea;
    static TimeT        sCollectiveStartTime;
};

#endif // WordRectFinder_hpp