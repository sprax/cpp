// WordTrie.cpp : word prefix tree with forward links (optimized for speed at the expense of memory)
// Sprax Lines, July 2010

#include "stdafx.h"
#include "WordTrie.hpp"

const WordNode* WordTrie::getNextWordNodeAndIndex(const WordNode *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const // fromBeg can change
{
   // We could memoize lazily using nextWordNode.  That is, either change this 
	// to start with the offending node and just set/get its nextWordNode (if the 
	// disqualified node is passed in!), or else replace the word member with a pointer to basically
    // an array of nextWordNodes for each length, so we just set/get/return nextWordNodes[fromBeg],
    // where fromBeg is rationalized to be in [0, wordLen).  But if we already have the offending
	// node, so we don't have to traverse (as below) to find it (or rather, its parent), then getting 
	// the next different stem is so simple that we don't really need to make a separate method for it.  
	// As it is, we don't memoize lazily, we just add all the forward links in addWord.

	// Anyway, here is the mon-memoized code.  It was used as a reference implementation before optimizing
	// all needed traversal using links.  I think it's slower than link-following by a factor of 2 or so.

	// Get the parent of the node containing the first 'bad' letter (if we already know it, we don't need to use this method).
    const WordNode *parent = NULL;
    if (fromBeg < fromEnd) {
        // Start at the root and find the starting parent by going deeper through children:
        parent = mRoot;
        for (int j = 0; j < fromBeg; j++) {
            parent = parent->getBranchAtLetter(keyWord[j]);
        }
    } else {
        // Start at the word and find the starting parent by going up through parents:
        parent = keyNode;
        while (--fromEnd >= 0) {
            parent = parent->getParent();
        }
    }
    assert(parent);

    while (fromBeg >= 0) {
        const WordNode *next = parent->getNextBranchFromLetter(keyWord[fromBeg]);
        if (next != NULL) {
            return next->getFirstWordNode();
        }
        fromBeg--;
        parent = parent->getParent();
    }
	fromBeg = 0;
    return NULL;
}