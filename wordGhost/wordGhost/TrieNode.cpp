// TrieNode.cpp : trie nodes for words
// Sprax Lines,  July 2010

#include "stdafx.h"
#include "TrieNode.hpp"
#include "WordMap.hpp"

const char *  TrieNode::sEmptyStr = "";

bool TrieNode::hasWord(const char *key) const
{
    // Traverse down the trie until we get to the end of the string:
    const TrieNode *node = this;
    for (const char *p = key; *p != NULL; p++) {
        node = node->mBranches[*p - sFirstLetter];
        if (node == NULL) {
            return  false;	 	// Not found - reached end of branch
        }
    }
    return (node->mWord != NULL);
}


TrieNode * TrieNode::addWord(const char *word, WordLenT, TrieNode *)
{
    // Traverse down the trie until we reach the end of the word, creating nodes as necessary.
    assert(word != NULL);
    TrieNode *node = this;
    for (const char *pc = word; ; ) {
        if (*pc == NULL) {          // End of the word.
            node->mWord = word;     // Don't _strdup or delete this word; its memory is owned by main & shared by others.
            return node;
        }
        int ic = *pc++ - sFirstLetter;
        assert(0 <= ic && ic < sNumLetters);
        if (node->mBranches[ic] == NULL) {
            node->mBranches[ic] =  new TrieNode(node->mDepth + 1, word);   // create new branch node as a child with a backpointer to its parent.
            // Add new node to linked list using iteration (we save memory by not storing a lastBranch)
            if (node->mFirstChild == NULL) {
                node->mFirstChild = node->mBranches[ic];
            } else for (int ib = ic; --ib >= 0; ) {
                if (node->mBranches[ib] != NULL) {
                    node->mBranches[ib]->mNextBranch = node->mBranches[ic];
                    break;
                }
            }
        }
        node = node->mBranches[ic];
        if (node->mStem == NULL)
            node->mStem =  word;			// pointer to the first or last word off this stem, it doesn't matter, they're same, up to depth characters
    }
    //  return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
}


const TrieNode* TrieNode::subTrix(const char *key, int idx) const		// deprecated, doesn't say where the key failed
{
    //  Follow the mBranches for each letter in the key up the specified index, and:
    //      return the last node, if key is a valid word stem, or 
    //      return NULL, if key is not a stem for any word in the trie.
    const TrieNode *node = this;
    do {
        node = node->getBranchAtLetter(*key++);
    } while (node != NULL && --idx >= 0);
    return node;
}

void TrieNode::printWords() const
{
    if (mWord != NULL) {
        printf("%s\n", mWord);
    }
    for (const TrieNode *node = getFirstChild(); node != NULL; node = node->getNextSibling() ) {
        node->printWords();
    }
}


