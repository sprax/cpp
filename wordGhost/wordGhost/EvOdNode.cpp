// EvOdNode.cpp : trie nodes for words
// Sprax Lines,  July 2010

#include "stdafx.h"
#include "EvOdNode.hpp"

EvOdNode * EvOdNode::addWordAndNumbersForward(const char *word, WordLenT wordLen)
{
    // Traverse down the trie until we reach the end of the word, creating nodes as necessary.
    // When we get to the word's end, add data specific to this type of tree/node.
    // Word extensions, that is, words that extend shorter words, are NOT added.
    if (word == NULL) {
        return  NULL;
    }
//#ifdef _DEBUG   // zoid
//    if (! strcmp(word, "rehiring")) {   // debugging
//        int rehiring = wordLen % 2;
//    }
//#endif

    EvOdNode * node = this;             // usually the root
    for (const char *pc = word; ; ) {
        if (*pc == NULL) {              // End of the word.
            node->setWord(word);        // Don't _strdup or delete this word; its memory is owned by main & shared by others.
            node->mEndWord  = word;
            node->mEndDepth = wordLen;
            for (EvOdNode * parent = node->getParent(); parent != this; parent = parent->getParent()) {
                if (! parent->updateNodeStatus() ) {
                    break;              // If this parent was not changed, it's parent won't change, either, so break
                }
            }
            return node;                // Done.
        }

        int ic = *pc++ - sFirstLetter;
        assert(0 <= ic && ic < sNumLetters);
        if (node->mBranches[ic] == NULL) {
            EvOdNode * child =  new EvOdNode(node, node->mDepth+1, word); // create new branch node as a child with a backpointer to its parent.
            node->mBranches[ic] = child;
            // Add new node to linked list using iteration (instead of using an mLastBranch, which is extra memory with no other use)
            if (node->mFirstChild == NULL) {
                node->mFirstChild = child;
            } else for (int ib = ic; --ib >= 0; ) {
                EvOdNode *lastBranch = node->getBranchAtIndex(ib);
                if (lastBranch != NULL) {
                    lastBranch->setNextBranch(child);
                    break;
                }
            }
        } else if (node->mBranches[ic]->isWord()) {
            // This word extends a word we already have.  We don't need it for games like Ghost...
            return NULL;
        }
        node = node->getBranchAtIndex(ic);
        //node->mIsWinner = isWinnerNodeD(node, node->mWinDepth = node->mDepth);

        if (node->mStem == NULL) {
            node->mStem =  word;			// pointer to the first or last word off this stem, it doesn't matter, they're same, up to depth characters
        }
    }
    //  return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
}


void EvOdNode::printChildWordsAndEndDepths() const
{
    if (mWord != NULL) {
        printf("%s  %d\n", mWord, mEndDepth);
    }
    for (const EvOdNode *node = getFirstChild(); node != NULL; node = node->getNextSibling() ) {
        node->printWords();
    }
}


/** Update this node's winner/loser status and its end word and end depth.
 *  Return true if this node is changed, false if not.
 */
bool EvOdNode::updateNodeStatus()
{   

    // For any winner node, node.mWinDepth == MAX{loser mBranches.mLoseDepth} === maxLosingChildrensLoseDepth
    // For any loser node, node.mLoseDepth == MIN{winner mBranches.mWinDepth} === minWinningChildrensWinDepth
    int numWinningChildren           = 0;
    WordLenT maxLosingChildrensLoseDepth  = 0;
    WordLenT minWinningChildrensWinDepth  = WORDLEN_MAX;
    const char *oLoseWord = NULL, *oWinWord = NULL;

    for (const EvOdNode *branch = getFirstChild(); branch != NULL; branch = branch->getNextSibling()) {
        if (branch->mIsWinner) {
            numWinningChildren++;
            if (minWinningChildrensWinDepth > branch->mEndDepth) {
                minWinningChildrensWinDepth = branch->mEndDepth;
                oWinWord = branch->getEndWord();
            }
        } else {    // MAX{loser mBranches.mLoseDepth} === maxLosingChildrensLoseDepth
            if (maxLosingChildrensLoseDepth < branch->mEndDepth) {
                maxLosingChildrensLoseDepth = branch->mEndDepth;
                oLoseWord = branch->getEndWord();
            }
        }
    }

    bool status = true;
    if (numWinningChildren == 0) {      // node is a winner
        // If node's status won't change, return false (at the end)
        if (mIsWinner == true && mEndDepth == maxLosingChildrensLoseDepth) {
            status = false;
        }
        // Update (actually change) status...
        mIsWinner   = true;
        mEndDepth = maxLosingChildrensLoseDepth;
        setEndWord(oLoseWord);
    } else {
        // If node's info won't change, return false (at the end)
        if (mIsWinner == false && mEndDepth == minWinningChildrensWinDepth) {
            status = false;
        }
        // Update (actually change) status...
        mIsWinner    = false;
        mEndDepth = minWinningChildrensWinDepth;
        setEndWord(oWinWord);
    }

#ifdef _DEBUG
    static int dbg = 0;
    if (dbg > 0) {
        if (getDepth() == 3) {
            printf("\n%5d %c %2d %c %2d %-18s %-14s %-14s %-14s %-14s  %2d\n"
                , dbg++
                , (mIsWinner ? 'w' : 'L')
                , getDepth()
                , getLetter()
                , strlen(getStem())
                , getWord()
                , (oWinWord ? oWinWord : "----") 
                , (oLoseWord  ? oLoseWord  : "----")
                ,  getEndWord()
                ,  mEndDepth
                );
        }
    }
#endif

    return status;
}

bool EvOdNode::evalIsWinner() const
{
    ADD_COUNT(1);

    // Any word-node is a loser.
    if (isWord()) {
        return false;
    }
    // If it isn't a word-node, it must have at least one branch,
    // and then it is a loser if any of its mBranches is a winner.
    const EvOdNode *branch = getFirstChild();
    do {
        if (branch->evalIsWinner()) {
            return false;
        }
    } while ((branch = branch->getNextSibling()) != NULL);
    
    // This node is not a word and all of its mBranches are losers, ergo, it is a winner:
    return true;
}

bool EvOdNode::evalIsWinnerGetEnds(WordLenT & rEndDepth, const char **pEndWord) const   // Only for unit testing!
{
    ADD_COUNT(1);

    // If this node is a word node, there are no children and there is nothing
    // left to find out: it's a loser of end depth = wordlength.
    if (isWord()) {
        rEndDepth = getDepth();
        *pEndWord = getWord();
        return false;
    }

    // If this node is not a word node, it must have at least one child,
    // and it is a loser iff any of its children is a winner.
    // So assume this node is a winner and iterate through its children.
    // As soon as one of its children turns out to be a winner, we know
    // this node is a loser, but keep we keep iterating to find the min/max/ends.
    // Because we don't yet know if this node is a winner/loser, we need to keep
    // both the max/winning depth and the min/losing depth.
    // If used two passes, the 1st to determine win/lose and the 2nd to find the depths or "ends",
    // we could track just one or the other.  But 2 passes feels like more overhead.
    bool nodeIsWinner     = true;       
    WordLenT maxWinDepth  = 0;
    WordLenT minLoseDepth = WORDLEN_MAX;
    const char * maxWinWord  = NULL;
    const char * minLoseWord = NULL;
    const EvOdNode *branch = getFirstChild();
    do {
        WordLenT tmpDepth = WORDLEN_MAX;
        const char *tmpWord = NULL;
        if (branch->evalIsWinnerGetEnds(tmpDepth, &tmpWord) ) {
            nodeIsWinner = false;
            if (minLoseDepth > tmpDepth) {
                minLoseDepth = tmpDepth;
                minLoseWord  = tmpWord;
            }
        } else if (maxWinDepth < tmpDepth) {
            maxWinDepth = tmpDepth;
            maxWinWord  = tmpWord;
        }
    } while ((branch = branch->getNextSibling()) != NULL);

    // The end-depth/end-word depend on whether this node is a winner/loser.
    if (nodeIsWinner) {
        rEndDepth = maxWinDepth;
        *pEndWord = maxWinWord;
    } else {
        rEndDepth = minLoseDepth;
        *pEndWord = minLoseWord;
    }
    return nodeIsWinner;
}




