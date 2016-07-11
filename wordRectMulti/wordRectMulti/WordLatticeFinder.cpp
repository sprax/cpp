// WordLatticeFinder.cpp : find word rectangle of the specified dimensions 
// Sprax Lines, July 2010

#include "WordLatticeFinder.hpp"
#include "WordRectPrinter.hpp"
#include "wordRectSearchMgr.hpp"

#if DEFINE_METHODS_IN_CPP
template <typename T>
int WordLatticeFinder<T>::findWordRows()
{
	return findWordRowsUsingTrieLinks(0);
}
#endif

template <typename T>
int WordLatticeFinder<T>::findWordRowsUsingTrieLinks(int haveTall)
{
    this->mNowTall = haveTall;
	if (haveTall > 1 && this->mWantArea <= WordRectSearchMgr<T>::getFoundArea()) {
        this->mState = WordRectFinder<T>::eAborted;
		return -this->mMaxTall;  // Abort because a wordRect bigger than wantArea has been found
	}

	int prevTall = haveTall - 1;              // prevTall == -1 is ok.
	const TrieNode *rowWordNode = NULL;
	if (haveTall == 0) {
		rowWordNode = this->mRowTrie.getFirstWordNode();
	} else if (haveTall < this->mWantTall) {
		uchr uc = this->mColNodes[0][prevTall]->getLetterAt(haveTall);
		uint ux = this->colCharIndex(uc);
		rowWordNode = this->mRowTrie.getFirstWordNodeFromIndex(ux);
		if (this->mMaxTall < haveTall)
			this->mMaxTall = haveTall;
	} else {
		// Success: the row just added made words of all columns.
		// Record it right now, right here.
		this->mMaxTall = haveTall;
		this->mNumWordRects++;
		for (int row = haveTall - 1, col = 0; col < this->mWantWide; col += 2) {
            this->mColWordsNow[col] = this->mColNodes[col][row]->getStem();  // Same as getFirstWord() in this context
		}
		for (int row = 0; row < haveTall; ++row)
			this->mRowWordsOld[row] = this->mRowWordsNow[row];
        this->mIsLastRectSymSquare = isSymmetricSquare();
        if (this->mIsLastRectSymSquare)
            this->mNumSymSquares++;
		if (this->mFindAll && WordRectSearchMgr<T>::getVerbosity() > 1) {
 			WordRectPrinter::printRect(this);
        }
		return this->mWantArea;                   // Return the area
	}

    const char *word;
	const TrieNode **rowNodes = this->mRowNodes[haveTall];
    for (int k = 0, kOld = 0, area = 0; rowWordNode != NULL; ) {
        word = rowWordNode->getWord();
        for (;;) {
            // For each stem (partial column) that would result from adding this word as the next row,
            // is there a word of length wantTall that could complete it?
            rowNodes[k] = rowNodes[k-1]->getBranchAtIndex(this->rowCharIndex(word[k]));                        // Need this later, so always set it.
            if (k % 2 == 0) {
                const TrieNode *colBranch = this->mColNodes[k][prevTall]->getBranchAtIndex(this->rowCharIndex(word[k]));   // Need this later only if non-NULL.
                if (colBranch == NULL) {
                    break;                              // ...if not, break to get the next word.
                }
                this->mColNodes[k][haveTall] = colBranch;      // ...if so, add the branch node and continue.
            }
            if (k == this->mWantWideM1) {
                // At least one wantTall-length word exists to complete each column, including this row,
                this->mRowWordsNow[haveTall] = word;                                 
                // so try adding another row.  Even rows are solid; odd rows alternate as "waffle ridges"
                //if (haveTall == mWantTall - 1)
                //    area = findWordRowsUsingTrieLinks(mWantTall);
                //else
                area = findWordColsUsingTrieLinks(haveTall, kOld);
                if (area < 0 || area > 0 && ! this->mFindAll) { // Either this call found a wordRect (area > 0),
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
		kOld = k;
		if (kOld % 2 == 1)
			kOld++;
		rowWordNode = nextStem->getFirstWordNode();
	}
	return 0;   // Falure: No wantWide x wantTall word rect at the end of this path
}


template <typename T>
int WordLatticeFinder<T>::findWordColsUsingTrieLinks(int row, int col)
{
	static int zoidDbg = 0;
	if (col >= this->mWantWide) {
		return findWordRowsUsingTrieLinks(row + 2);
	}

	const TrieNode *colNode = this->mColNodes[col][row];
	const TrieNode *child  = colNode->getFirstChild();
	for (int rowP1 = row + 1; child != NULL; child = child->getNextBranch()) {
		this->mColNodes[col][rowP1] = child;
    	if (col == 2) {
	    	zoidDbg++;
	    }
		int area =  findWordColsUsingTrieLinks(row, col + 2);
		if (area < 0 || area > 0 && ! this->mFindAll) { // Either this call found a wordRect (area > 0),
			return area;                      // or it is aborting (because another thread found one bigger
		}                                   // than this one's wantArea); so return up through the stack
	}
    return 0;   // Falure: No wantWide x wantTall word rect at the end of this path
}




template <typename T>
int WordLatticeFinder<T>::findWordRowsUsingTrieLinks_iterTodo(int haveTall)
{
    static int zoidDbg = 0;

    this->mNowTall = haveTall;
	if (haveTall > 1 && this->mWantArea <= this->getFoundArea()) {
        this->mState = WordRectFinder<T>::eAborted;
		return -this->mMaxTall;  // Abort because a wordRect bigger than wantArea has been found
	}

	int prevTall = haveTall - 1;
	const TrieNode *rowWordNode = NULL;
	if (haveTall == 0) {
		rowWordNode = this->mRowTrie.getFirstWordNode();
	} else if (haveTall < this->mWantTall) {
        const TrieNode *firstColNode = this->mColNodes[0][prevTall];
        if (haveTall + 1 < this->mWantTall)
            rowWordNode  = this->mRowTrie.getFirstWordNodeFromIndex(rowCharIndex(this->mColNodes[0][prevTall]->getLetterAt(haveTall+1)));
		else
            rowWordNode = this->mRowTrie.getFirstWordNode();

   		const TrieNode *rowWordNodeB = firstColNode->getFirstChild()->getFirstWordNode();
        if (rowWordNode != rowWordNodeB) {
            zoidDbg++;
        }

		if (this->mMaxTall < haveTall)
			this->mMaxTall = haveTall;
	} else {
		// Success: the row just added made words of all columns.
		// Record it right now, right here.
		this->mMaxTall = haveTall;
		this->mNumWordRects++;
		for (int row = 0; row < haveTall; ++row)
			this->mRowWordsOld[row] = this->mRowWordsNow[row];
        this->mIsLastRectSymSquare = isSymmetricSquare();
        if (this->mIsLastRectSymSquare)
            this->mNumSymSquares++;
		if (this->mFindAll && WordRectSearchMgr<T>::sVerbosity > 1) {
 			PrintWordRectViaSearchMgr(this);
        }
		return this->mWantArea;                   // Return the area
	}

    if (haveTall % 2 == 0) {
        const TrieNode **rowNodes = this->mRowNodes[haveTall];
        for (int k = 0; rowWordNode != NULL; ) {
            const char *word = rowWordNode->getWord();
            for (;;) {
                // For each stem (partial column) that would result from adding this word as the next row,
                // is there a word of length wantTall that could complete it?
                rowNodes[k] = rowNodes[k-1]->getBranchAtIndex(this->rowCharIndex(word[k]));                        // Need this later, so always set it.

                if (k % 2 == 0) {
                    const TrieNode *colNodeK = this->mColNodes[k][prevTall];
                    const TrieNode *branch = colNodeK->getFirstChild();
                    const TrieNode *colBranch = NULL;
                    for ( ; branch != NULL; branch = branch->getNextBranch()) {
                        colBranch = branch->getBranchAtIndex(this->colCharIndex(word[k]));   // Need this later only if non-NULL.
                        if (colBranch != NULL) 
                            break;
                    }
                    if (colBranch == NULL) {
                        break;                              // ...if not, break to get the next word.
                    }
                    this->mColNodes[k][haveTall] = colBranch;      // ...if so, add the branch node and continue.
                }
                if (k >= this->mWantWideM1) {
                    // At least one wantTall-length word exists to complete each column, including this row,
                    this->mRowWordsNow[haveTall] = word;                                 // so try adding another row.
                    int area =  findWordRowsUsingTrieLinks(haveTall + 1);
                    if (area < 0 || area > 0 && ! this->mFindAll) { // Either this call found a wordRect (area > 0),
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
    } else { // haveTall is odd
       for (int k = 0; k < this->mWantWide; k += 2) {
           this->mColNodes[k][haveTall] = this->mColNodes[prevTall][k]->getFirstChild();
       }

    }
    return 0;   // Falure: No wantWide x wantTall word rect at the end of this path
}

template <typename T>
bool WordLatticeFinder<T>::isSymmetricSquare() const
{
    if (this->mWantTall != this->mWantWide)
        return false;                           // not square
    for (int row = 0; row < this->mWantTall; ++row) {
        for (int col = this->mWantWide; --col >= 0; ) {
            if (this->mRowNodes[row][col] != this->mColNodes[row][col])
                return false;                   // not symmetric
        }
        if (++row == this->mWantTall)
            break;
        for (int col = 0; col < this->mWantWide; col += 2) {
            if (this->mRowNodes[row][col] != this->mColNodes[row][col])
                return false;                   // not symmetric
        }
    }
    return true;                                // square and symmetric
}


/** Print actual word rows of complete or in-progress word rectangle. */
template <typename T>
void WordLatticeFinder<T>::printWordRows(const char *wordRows[], int haveTall) const
{
    for (int row = haveTall - 1, col = 0; col < this->mWantWide; col += 2) {
        this->mColWordsNow[col] = this->mColNodes[col][row]->getStem(); // same as getFirstWord() in this context
    }

    for (int row = 0; row < haveTall; ++row) {
        for (const char *pc = wordRows[row]; *pc != '\0'; pc++) {
            printf(" %c", *pc);
        }
        printf("\n");
        if (++row == haveTall)
            break;
        for (int col = 0; col < this->mWantWide; col += 2) {
            printf(" %c  ", this->mColWordsNow[col][row]);
        }
        printf("\n");
    }
 /*   printf("\n");
    WordRectFinder::printWordRows(wordRows, haveTall);
    printf("\n");*/
}

/*
a b a c a b a
b   b   b   b
a b a c a b a
c   c   c   c
a b a c a b a
b   b   b   b
a b a c a b a
*/