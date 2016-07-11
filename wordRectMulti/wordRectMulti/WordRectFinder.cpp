// WordRectFinder.cpp : find word rectangle of the specified dimensions 
// Sprax Lines, July 2010

#include "WordTrie.hpp"
#include "WordRectFinder.hpp"
#include "WordRectPrinter.hpp"
#include "WordRectSearchMgr.hpp"

template <typename T> 
WordRectFinder<T>::WordRectFinder(int wide, int tall, unsigned int options, WordTrie<T> *wordTries[], const WordMap *maps) 
	: mRowTrie(*wordTries[wide])
	, mColTrie(*wordTries[tall])
	, mWantWide(wide)
	, mWantTall(tall)
	, mWantWideM1(wide - 1)
	, mWantArea(wide * tall)
	, mFindAll(options & eFindAll ? true : false)
	, mUseMaps(options & eUseMaps ? true : false)
	, mUseAltA(options & eUseAltAlg ? true : false)
	, mMaxTall(0), mNowTall(0), mNumWordRects(0), mNumSymSquares(0)
	, mIsLastRectSymSquare(false), mRowWordsOld(NULL)
	, mStartTime(-1), mState(eReady), mWordMaps(maps), mRowWordsNow(NULL)   
	// start time < 0 means never; maps used as an option
{ }

template <typename  T> 
void WordRectFinder<T>::initRowsAndCols()
{
	if (mRowWordsNow != NULL)
		return;

	mRowWordsNow = (const char **)malloc(mWantTall * sizeof(char *));
	mRowWordsOld = (const char **)malloc(mWantTall * sizeof(char *));
	mRowNodes    = (const TrieNode ***)malloc(mWantTall * sizeof(TrieNode **));
	mColNodes    = (const TrieNode ***)malloc(mWantWide * sizeof(TrieNode **));

	// For each row, make rowNodes[row] point to the start of the word row,
	// which is one pointer address *after* the row's raw memory start, 
	// and make rowNodes[row][-1] point to the mRowTrie root.  This memory
	// offset allows word-node indexing to agree with word-character indexing.
	int wideP1 = mWantWide + 1;
	int tallP1 = mWantTall + 1;
	mRowNodesMem = (const TrieNode  **)malloc(wideP1 * mWantTall * sizeof(TrieNode *));
	for (int row = 0; row < mWantTall; row++) {
		mRowNodes[row]     = &mRowNodesMem[row * wideP1 + 1];
		mRowNodes[row][-1] =  mRowTrie.getRoot();
	}

	// For each column, make colNodes[col] point to the start of the column,
	// which is one pointer address *after* the column's raw memory start, 
	// and make colNodes[col][-1] point to the mColTrie root.
	mColNodesMem = (const TrieNode  **)malloc(mWantWide * tallP1 * sizeof(TrieNode *));
	for (int col = 0; col < mWantWide; col++) {
		mColNodes[col]     = &mColNodesMem[col * tallP1 + 1];
		mColNodes[col][-1] = mColTrie.getRoot();
	}
}




template <typename T> 
int WordRectFinder<T>::findWordRows()
{
	if (mUseMaps && mWordMaps != NULL) {
		char  wordCols[MAXLEN][MAXLEN];
		const WordMap & rowMap = mWordMaps[ getWide() ];
		return findWordRectRowsMapUpper(0, rowMap, wordCols);
	}
    return findWordRowsUsingTrieLinks(0);
}

template <typename T> 
int WordRectFinder<T>::findWordRowsUsingTrieLinks(int haveTall)
{
    mNowTall = haveTall;
	if (haveTall > 1 && mWantArea <= WordRectSearchMgr<T>::getFoundArea()) {
        mState = eAborted;
		return -mMaxTall;  // Abort because a wordRect bigger than wantArea has been found
	}

	int prevTall = haveTall - 1;              // prevTall == -1 is ok.
	const TrieNode *rowWordNode = NULL;
	if (haveTall == 0) {
		rowWordNode = mRowTrie.getFirstWordNode();
	} else if (haveTall < mWantTall) {
        uchr letter = mColNodes[0][prevTall]->getLetterAt(haveTall);
        assert(mRowTrie.charMap().sourceMinChar() <= letter && letter <= mRowTrie.charMap().sourceMaxChar());
        uint index = rowCharIndex(letter);
		rowWordNode = mRowTrie.getFirstWordNodeFromIndex(index);
		if (mMaxTall < haveTall)
			mMaxTall = haveTall;
	} else {
		// Success: the row just added made words of all columns.
		// Record it right now, right here.
		mMaxTall = haveTall;
		mNumWordRects++;
		for (int row = 0; row < haveTall; ++row)
			mRowWordsOld[row] = mRowWordsNow[row];
        mIsLastRectSymSquare = isSymmetricSquare();
        if (mIsLastRectSymSquare)
            mNumSymSquares++;
		if (mFindAll && WordRectSearchMgr<T>::getVerbosity() > 1) {
			//WordRectSearchMgr<T>::printWordRectMutex(this);
			WordRectPrinter::printRect(this);
        }
		return mWantArea;                   // Return the area
	}

	const TrieNode **rowNodes = mRowNodes[haveTall];
	for (int k = 0; rowWordNode != NULL; ) {
		const char *word = rowWordNode->getWord();
        // For each vertical stem (partial column) that would result from adding this word
		// as the next row, is there a whole word of length wantTall that could complete it?
		// If yes (IFF the implied colNode is non-null), continue; else break and get next word.
 		for (;;) {
			// We'll need rowNode[k] later; we'll need the colNode later only if it is non-NULL
            uchr letter = word[k];
            uint index  = rowCharIndex(letter);
			rowNodes[k] = rowNodes[k-1]->getBranchAtIndex(index);
			const TrieNode *colBranch = mColNodes[k][prevTall]->getBranchAtIndex(index);
			if (colBranch == NULL) {
				break;                              // ...if not, break to get the next word.
			}
			mColNodes[k][haveTall] = colBranch;		// ...if so, add the branch node and continue.
			if (k == mWantWideM1) {
				// At least one wantTall-length word exists to complete each column, including this row,
				mRowWordsNow[haveTall] = word;                                 // so try adding another row.
				int area =  findWordRowsUsingTrieLinks(haveTall+1);
				if (area < 0 || area > 0 && ! mFindAll) { // Either this call found a wordRect (area > 0),
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
		// it just by following 2 links:  (failed node)->nextStemNode->firstWordNode.
		// (If we weren't using the rowNodes, we'd need to find the parent of the first
		// disqualified node, as does WordTrie::getNextWordNodeAndIndex, but over all, that is slower.)
		const TrieNode * nextStem = rowNodes[k]->getNextStemNode();
		if (nextStem == NULL) {
			return 0;   // We've tried all possible word stems for the partial columns we got, so return. 
		}
		k = nextStem->getDepth() - 1;
		rowWordNode = nextStem->getFirstWordNode();
	}
	return 0;   // Falure: No wantWide x wantTall word rect at the end of this path
}


#ifdef              findWordRectRowsUsingGetNextWordNodeAndIndex  // slower than links, faster than upper_bound; left here as a reference implementation
int WordRectFinder::findWordRectRowsUsingGetNextWordNodeAndIndex(  int wantWide, int wantTall, int haveTall
	, const char *wordRows[MAXLEN], const TrieNode ** colNodes[BUFSIZE] )
{
	if (haveTall == wantTall) {
		return wantWide*wantTall;
	}
	HaveTall = haveTall;

	int k = 0, prevTall = haveTall - 1;	            // prevTall == -1 is ok.
	const TrieNode *rowWordNode = (haveTall > 0) ?
		mRowTrie.getFirstWordNodeFromLetter(colNodes[0][prevTall]->getLetterAt(haveTall)) : 
	mRowTrie.getFirstWordNode();
	while(rowWordNode != NULL) {
		const char *word = rowWordNode->getWord();
		for ( ; k < wantWide; k++) {
			// For each resulting stem (column), is there a word of length wantTall that could complete it?
			const TrieNode *colBranch = colNodes[k][prevTall]->getBranchAtLetter(word[k]);
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

template <typename T> 
int WordRectFinder<T>::findWordRectRowsMapUpper(int haveTall, const WordMap& rowMap, char wordCols[][MAXLEN])
{
	mNowTall     = haveTall;                // rectangle height == stack height
	int wantWide = mRowTrie.getWordLength();
	int wantTall = mColTrie.getWordLength();
	if (wantTall == haveTall) {            // Success: the row just added made words of all columns   
		return mWantArea;                   // Return the area
	}
	if (haveTall > 2 && mWantArea <= WordRectSearchMgr<T>::getFoundArea()) {
		return -mWantArea;                  // Abort because a wordRect bigger than wantArea has been found
	}

	char temp[BUFSIZE];

    int area = 0;
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
		mRowWordsNow[haveTall] = word;
		area = findWordRectRowsMapUpper(haveTall+1, rowMap, wordCols);
		if (area > 0) {
			return area;
		}
		++itr;
END_LOOP:;
	}
	return 0;
}

/*
 s t a m m e l t
 t a b u e r e r
 a b b r i n g e
 m u r k s t e n
 m e i s t e r n
 e r n t e t e t
 l e g e r e r e
 t r e n n t e n
*/