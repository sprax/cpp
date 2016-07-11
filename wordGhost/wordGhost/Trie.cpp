// WordTrie.cpp : word prefix tree with forward links (optimized for speed at the expense of memory)
// Sprax Lines, July 2010

#include "stdafx.h"
#include "Trie.hpp"

#ifdef _DEBUG   // not referenced
static int initBaseTrieFromSortedDictionaryFile(Trie<TrieNode> *trie, WordMap *wordMap, const char *fname) // TODO: expose this?
{   
    // This method allocates memory for holding all the words, and that memory is retained 
    // until the process terminates.  So it should succeed only once.       
    // TODO: Implement a clear or reset method that unloads the trie?  
    // It could just delete everything in the trie, but other methods don't check for this condition.
    if (trie->getAlreadyLoaded()) {
        return 0;
    }

    FILE *fi;
    if ( ! fopen_s(&fi, fname, "r") ) {
        printf("Opened %s . . . " , fname);
    } else {
        printf("Error opening dictionary file: %s\n", fname);
        return 0;
    }

    TrieNode *prevNode = NULL;
    char word[BUFSIZE];
    int numWords = 0, maxWordLen = 0;
    while (fgets(word, BUFSIZE, fi))  {
        // got next word as a line in the file; now get its length, and remove any trailing newline.
        WordLenT length = (WordLenT)strlen(word);
        WordLenT lengthM1  = length - 1;
        if (word[lengthM1] < WordNode::sFirstLetter) {
            word[lengthM1] = NULL;
            length = lengthM1;
        }
        if (maxWordLen < length)  {
            maxWordLen = length;
        }
        char *saveWord = _strdup(word);		// Allocate word's memory here, for the life of the program.
        numWords++;
        prevNode = trie->addWord(saveWord, length, prevNode);
        if (wordMap != NULL) {
            assert(length > 0);
            assert(length <= MAXLEN);
            wordMap->insert(WordMap::value_type(saveWord, numWords));   // strictly increasing value preserves the order
        }
    }
    trie->setAlreadyLoaded(true);
    fclose(fi);
    printf("Found %d words, max word length %d\n", numWords, maxWordLen);
    return maxWordLen;
}
#endif

// TODO: Fix ONE of these methods, verify that it really is WAY more efficient, 
// then put it back in EvOdNode.cpp where it belongs.


// POV: We are just initializing our trie now, but later, during the game, 
// I as a player may be given this node, meaning my opponent chose it, 
// so I win/lose iff this node->mIsWinner == false/true.  (I can win if you chose a loser node.)  
// So we continue initializing this node now in such a way 
// that it will be trivial for me to choose what to do when 
// given this node in the game.
// This node is a winner now, so all its children are losers, but
// this node is about to get a new child that is a winner, 
// so we flip its status to loser.
// Then, if given this node in the game, I will win (because I can choose this newly added winning child). 
// To win, in general, I pick any of this node's winning children that has a minimum mWinDepth.  
// As of now, there will only be one winning child, the new one, 
// so we use this child's info to update this node's mLoseDepth, etc.
//
// Then, apply consequences to chain of parents:
// A) if this node's parent had other winner children, the parent remains a loser,
// but its mLoseDepth must be re-selected to be MAX{parent's remaining winning children's mWinDepths}.
// B) if this node was its parent's only winner child,
// the parent now flips to become a winner, and the parent's
// mWinDepth must be set to MIN{parent's loser children's mLoseDepth},
// which, if it was already up-to-date, means
// parent->mWinDepth = wordLen < parent->mWinDepth ? wordLen : parent->mWinDepth;
//
// THEN, IFF this parent's mIsWinner AND/OR mLoseDepth actually changed do we continue upward!



EvOdNode * EvOdNode::addWordAndNumbersBackUp(const char *word, WordLenT, EvOdNode *)
{
    // Traverse down the trie until we reach the end of the word, creating nodes as necessary.
    // When we get to the word's end, add data specific to this type of tree/node.
    if (word == NULL) {
        return  NULL;
    }
    EvOdNode * node = this;             // usually the root
    for (const char *pc = word; ; ) {
        if (*pc == NULL) {              // End of the word.
            node->setWord(word);        // Don't _strdup or delete this word; its memory is owned by main & shared by others.
            WordLenT nodeDepth = node->getDepth();
            // Now set the winning/losing status for this node and its ancestors: If I am this node, there are 4 cases:
            // 1.   I'm a winner & my parent is a loser:  continue upwards, because we may need to flip my grandparent, great-great-grandparent, etc.
            // 2.   I'm a loser & my parent is a winner:  break, because we don't have sufficient info to flip my grandparent, etc.
            // 3.   I'm a winner & my parent is a winner: Set my parent to loser and keep going up.
            // 4.   I'm a loser & my parent is a loser:   Set my parent to winner and keep going up, IFF all my siblings are losers.
            //      Since all nodes are initially losers, the first word found for each stem will propagate up as far as needed.
            // Cases 1 & 2 cannot be combined, becuase one breaks out of the loop, and the other continues upward.
            bool bWinner = false;   // We start with this word node, which is a loser; its mIsWinner was already initialized as false.
            int oSteps = 2;
            EvOdNode *child = node;
            for (EvOdNode *parent = node->getParent(); parent != NULL; child = parent, parent = parent->getParent()) {

                oSteps++;

                if (bWinner && ! parent->mIsWinner) {         // This is case 1.
                    if (parent->mEndDepth < nodeDepth) {
                        parent->mEndDepth = nodeDepth;
                    }
                    bWinner = false;
                    continue;                               // propagate up
                }
                if(!bWinner && parent->mIsWinner) {           // This is case 2.
                    bWinner = true;
                    if (parent->mEndDepth < nodeDepth) {
                        parent->mEndDepth = nodeDepth;
                    }
                    continue;   // break;                              // don't need to propagate upwards?
                }

                if (parent->mIsWinner == true) {          // This is case 3.  We know bWinner is also true w/o testing it again.
                    parent->mIsWinner = bWinner = false;  // Parent cannnot be a winner if any of its children are losers.  Flip bWinner, too.

                    if (parent->mEndDepth < nodeDepth) {
                        parent->mEndDepth = nodeDepth;
                    }

                    continue;
                }


                // That leaves us in case 4: my parent and I are both marked as losers now, but if all my siblings are also losers, 
                // we change the parent into a winner.  We know our parent has at least one branch (this child), 
                // and because we are loading from a pre-sorted dictionary file, this one is the last child.
                for (const EvOdNode *branch = parent->getFirstChild(); branch != NULL; branch = branch->getNextSibling()) {  // != child
                    if (branch->mIsWinner) {
                        goto NEXT_PARENT;               // break out of inner loop and continue to next iteration of outer loop.
                    }
                }
                parent->mIsWinner  = bWinner = true;       // all of parent's children are losers, so flip the parent to winner

                if (parent->mEndDepth < nodeDepth) {
                    parent->mEndDepth = nodeDepth;
                }

NEXT_PARENT:;
            }


            return node;            // Done.
        }
        int ic = *pc++ - sFirstLetter;
        assert(0 <= ic && ic < sNumLetters);
        if (node->mBranches[ic] == NULL) {
            node->mBranches[ic] =  new EvOdNode(node, node->mDepth + 1, word);   // create new branch node as a child with a backpointer to its parent.
            // Add new node to linked list using iteration (instead of using an mLastBranch, which is extra memory with no other use)
            if (node->mFirstChild == NULL) {
                node->mFirstChild = node->mBranches[ic];
            } else for (int ib = ic; --ib >= 0; ) {
                EvOdNode *lastBranch = node->getBranchAtIndex(ib);
                if (lastBranch != NULL) {
                    lastBranch->setNextBranch(node->getBranchAtIndex(ic));
                    break;
                }
            }
        } else if (node->mBranches[ic]->isWord()) {
            // This word extends a word we already have.  We don't need it for games like Ghost...
            return NULL;
        }
        node = node->getBranchAtIndex(ic);

        if (node->mStem == NULL) {
            node->mStem =  word;			// pointer to the first or last word off this stem, it doesn't matter, they're same, up to depth characters
        }
    }
    //  return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
}

#ifdef _DEBUG   // not used (yet)
EvOdNode * EvOdNode::addWordAndWinLose(const char *word, WordLenT, EvOdNode *)
{
    // Traverse down the trie until we reach the end of the word, creating nodes as necessary.
    // When we get to the word's end, add data specific to this type of tree/node.
    assert(word != NULL);

    EvOdNode * node = this;
    for (const char *pc = word; ; ) {
        if (*pc == NULL) {          // End of the word.
            node->setWord(word);    // Don't _strdup or delete this word; its memory is owned by main & shared by others.

            // Now set the winning/losing status for this node and its ancestors: If I am this node, there are 4 cases:
            // 1.   I'm a winner & my parent is a loser:  continue upwards, because we may need to flip my grandparent, great-great-grandparent, etc.
            // 2.   I'm a loser & my parent is a winner:  break, because we don't have sufficient info to flip my grandparent, etc.
            // 3.   I'm a winner & my parent is a winner: Set my parent to loser and keep going up.
            // 4.   I'm a loser & my parent is a loser:   Set my parent to winner and keep going up, IFF all my siblings are losers.
            //      Since all nodes are initially losers, the first word found for each stem will propagate up as far as needed.
            // Cases 1 & 2 cannot be combined, becuase one breaks out of the loop, and the other continues upward.
            bool bWinner = false;   // We start with this word node, which is a loser; its mIsWinner was already initialized as false.
            for (EvOdNode *parent = node->getParent(); parent != NULL; parent = parent->getParent()) {
                if ( ! parent->mIsWinner && bWinner) {    // This is case 1.
                    bWinner = false;
                    continue;                           // propagate up
                }
                if (parent->mIsWinner && ! bWinner) {     // This is case 2.
                    bWinner = true;
                    break;                              // don't need to propagate upwards
                }
                if (parent->mIsWinner) {                  // This is case 3.  We know bWinner is also true w/o testing it again.
                    parent->mIsWinner = bWinner = false;  // Parent cannnot be a winner if any of its children are losers.  Flip bWinner, too.
                    continue;
                }

                // That leaves us in case 4: my parent and I are both marked as losers now, but if all my siblings are also losers, 
                // we change the parent into a winner.  We know our parent has at least one branch (this child), 
                // and because we are loading from a pre-sorted dictionary file, this one is the last child.
                for (const EvOdNode *branch = parent->getFirstChild(); branch != NULL; branch = branch->getNextSibling()) {
                    if (branch->mIsWinner) {
                        goto NEXT_PARENT;               // break out of inner loop and continue to next iteration of outer loop.
                    }
                }
                parent->mIsWinner = bWinner = true;       // all of parent's children are losers, so flip the parent to winner
NEXT_PARENT:;
            }


            return node;            // Done.
        }
        int ic = *pc++ - sFirstLetter;
        assert(0 <= ic && ic < sNumLetters);
        if (node->mBranches[ic] == NULL) {
            node->mBranches[ic] =  new EvOdNode(node, node->mDepth + 1, word);   // create new branch node as a child with a backpointer to its parent.
            // Add new node to linked list using iteration (instead of using an mLastBranch, which is extra memory with no other use)
            if (node->mFirstChild == NULL) {
                node->mFirstChild = node->mBranches[ic];
            } else for (int ib = ic; --ib >= 0; ) {
                EvOdNode *lastBranch = node->getBranchAtIndex(ib);
                if (lastBranch != NULL) {
                    lastBranch->setNextBranch(node->getBranchAtIndex(ic));
                    break;
                }
            }
        } else if (node->mBranches[ic]->isWord()) {
            // This word extends a word we already have.  We don't need it for games like Ghost...
            return NULL;
        }
        node = node->getBranchAtIndex(ic);

        if (node->mStem == NULL) {
            node->mStem =  word;			// pointer to the first or last word off this stem, it doesn't matter, they're same, up to depth characters
        }
    }
    //  return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
}
#endif

