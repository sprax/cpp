// WordRectFinder.cpp : find word rectangle of the specified dimensions 
// Sprax Lines, July 2010

#include "stdafx.h"
#include "WordRectFinder.hpp"

int     WordRectFinder::sFoundArea              = 0;
int     WordRectFinder::sMinimumArea            = 0;
TimeT  WordRectFinder::sCollectiveStartTime    = 0;

int WordRectFinder::doSearch() {
    mStartTime = GetTime();
    if (mWordMaps!= NULL) {
        char  wordCols[MAXLEN][MAXLEN];
        const WordMap & rowMap = mWordMaps[ getWide() ];
        return findWordRectRowsMapUpper(0, rowMap, wordCols);
    } else {
        return findWordRectRowsUsingTrieLinks(0);
    }
}

void WordRectFinder::printWordRect(bool complete, TimeT timeNow, int finderIdx) const
{
    // Show complete or in-progress word rectangle. 
    static const int offset = 'A' - 'a';
    int wantWide = getWide();
    int haveTall = mHaveTall;   // snapshot, because mHaveTall is prone to change between printf's
    if (complete) {
        printf("FOUND  %2d * %2d wordRect, area %3d in %2d seconds in thread %2d:\n"
            , wantWide, haveTall, wantWide*haveTall, DiffTimeSeconds(timeNow, mStartTime), finderIdx);
    } else {
        printf("Trying %2d * %2d & found %2d rows after %2d seconds in thread %2d:\n", wantWide, getTall()
            , haveTall, DiffTimeSeconds(timeNow, mStartTime), finderIdx);
    }
    for (int k = 0; k < haveTall; k++) {
        for (const char *pc = mWordRows[k]; *pc != NULL; pc++) {
            printf(" %c", *pc + offset);
        }
        printf("\n");
    }
}

int WordRectFinder::findWordRectRowsUsingTrieLinks(int haveTall)
{
    mHaveTall    = haveTall;                // rectangle height == stack height
    int wantWide = mRowTrie.getWordLen();
    int wantTall = mColTrie.getWordLen();
    if (wantTall == mHaveTall) {            // Success: the row just added made words of all columns   
        return mWantArea;                   // Return the area
    }
    if (haveTall > 2 && mWantArea <= getFoundArea()) {
        return -mWantArea;                  // Abort because a wordRect bigger than wantArea has been found
    }

    int prevTall = haveTall - 1, wantWideM1 = wantWide - 1;              // prevTall == -1 is ok.
    const WordNode *rowNodesMem[BUFSIZE], **rowNodes = &rowNodesMem[1];  // put row nodes on the local stack: 1 row per stack level.
    rowNodes[-1] = mRowTrie.getRoot();
    const WordNode *rowWordNode = (haveTall > 0) ?
        mRowTrie.getFirstWordNodeFromLetter(mColNodes[0][prevTall]->getLetterAt(haveTall)) : 
    mRowTrie.getFirstWordNode();
    for (int k = 0; rowWordNode != NULL; ) {
        const char *word = rowWordNode->getWord();
        for (;;) {
            // For each stem (partial column) that would result from adding this word as the next row,
            // is there a word of length wantTall that could complete it?
            rowNodes[k] = rowNodes[k-1]->getBranchAtLetter(word[k]);                        // Need this later, so always set it.
            const WordNode *colBranch = mColNodes[k][prevTall]->getBranchAtLetter(word[k]);   // Need this later only if non-NULL.
            if (colBranch == NULL) {
                break;                              // ...if not, break to get the next word.
            }
            mColNodes[k][haveTall] = colBranch;      // ...if so, add the branch node and continue.
            if (k == wantWideM1) {
                // At least one wantTall-length word exists to complete each column, including this row,
                mWordRows[haveTall] = word;                                 // so try adding another row.
                int area =  findWordRectRowsUsingTrieLinks(haveTall+1);
                if (area != 0) {                    // Either this call found a wordRect (area > 0),
                  return area;                      // or it is aborting (because another thread found one bigger
                }                                   // than this one's wantArea); so return up through the stack
                break;                              // This word failed, so break to get the next word.
            }
            k++;
        }
        // As a candidate for wordRows[haveTall], this word failed at column k,
        // either locally (no completion for column k < wantWide-1) or deeper down
        // in the wordRect (all wantWide columns were ok, some later word row could not be found).
        // Either way, we try the next possible word, which is the first word whose 
        // k-letter stem is greater than this word's.  If such a word exists, we get 
        // it just by following 2 links:  (failed node)->mNextStemNode->mFirstWordNode.
        // (If we weren't using the rowNodes, we'd need to find the parent of the first
        // disqualified node, as does WordTrie::getNextWordNodeAndIndex, but over all, that is slower.)
        const WordNode * nextStem = rowNodes[k]->getNextStemNode();
        if (nextStem == NULL) {
            return 0;   // We've tried all possible word stems for the partial columns we got, so return. 
        }
        k = nextStem->getDepth() - 1;
        rowWordNode = nextStem->getFirstWordNode();
    }
    return 0;           // Falure: found no wantWide x wantTall word rect. 
}

#ifdef              findWordRectRowsUsingGetNextWordNodeAndIndex  // slower than links, faster than upper_bound; left here as a reference implementation
int WordRectFinder::findWordRectRowsUsingGetNextWordNodeAndIndex(  int wantWide, int wantTall, int haveTall
    , const char *wordRows[MAXLEN], const WordNode ** colNodes[BUFSIZE] )
{
    if (haveTall == wantTall) {
        return wantWide*wantTall;
    }
    HaveTall = haveTall;

    int k = 0, prevTall = haveTall - 1;	            // prevTall == -1 is ok.
    const WordNode *rowWordNode = (haveTall > 0) ?
        mRowTrie.getFirstWordNodeFromLetter(colNodes[0][prevTall]->getLetterAt(haveTall)) : 
    mRowTrie.getFirstWordNode();
    while(rowWordNode != NULL) {
        const char *word = rowWordNode->getWord();
        for ( ; k < wantWide; k++) {
            // For each resulting stem (column), is there a word of length wantTall that could complete it?
            const WordNode *colBranch = colNodes[k][prevTall]->getBranchAtLetter(word[k]);
            if (colBranch == NULL) {
                break;                              // ...if not, break (and try the next word)
            }
            colNodes[k][haveTall] = colBranch;      // ...if so, add the branch node and continue.
        }

        // At least one wantTall-length word exists to complete each column, so try adding another row.
        if (k == wantWide) {
            wordRows[haveTall] = word;
            int area = findWordRectRowsUsingGetNextWordNodeAndIndex(wantWide, wantTall, haveTall+1, wordRows, colNodes);
            if (area > 0) {
                return area;
            }
            k = wantWide - 1;	// limit to the correct range
        }
        // As a candidate for wordRows[haveTall], this word failed at column k: 
        // no wantTall-letter word begins with the haveTall letters in in the resulting column.  
        // So try the next word of length wantWide with stem > word-up-to-length-k.
        rowWordNode = mRowTrie.getNextWordNodeAndIndex(rowWordNode, word, k, wantWide - k);
    }
    return 0;
}
#endif


int WordRectFinder::findWordRectRowsMapUpper(int haveTall, const WordMap& rowMap, char wordCols[][MAXLEN])
{
    mHaveTall    = haveTall;                // rectangle height == stack height
    int wantWide = mRowTrie.getWordLen();
    int wantTall = mColTrie.getWordLen();
    if (wantTall == mHaveTall) {            // Success: the row just added made words of all columns   
        return mWantArea;                   // Return the area
    }
    if (haveTall > 2 && mWantArea <= getFoundArea()) {
        return -mWantArea;                  // Abort because a wordRect bigger than wantArea has been found
    }

    char temp[BUFSIZE];

    for(WordMap::const_iterator itr = rowMap.begin(), end = rowMap.end(); itr != end; ) {
        const char *word = itr->first;
        for (int k = 0; k < wantWide; k++) {
            wordCols[k][haveTall] = temp[k] = word[k];
            if ( ! mColTrie.subTrix(wordCols[k], haveTall) ) {
                temp[k+1] = '{';				// ASCII decimal 123, the char after 'z'
                temp[k+2] = '\0';				// NULL-terminate the C-string
                itr = rowMap.upper_bound(temp);	// Get first word-node alphabetically > temp
                goto END_LOOP;          		// break out of both inner and outer loop
            }
        }
        mWordRows[haveTall] = word;
        int area = findWordRectRowsMapUpper(haveTall+1, rowMap, wordCols);
        if (area > 0) {
            return area;
        }
        ++itr;
END_LOOP:;
    }
    return 0;
}
