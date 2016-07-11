// TrieNode.cpp : trie nodes for words
// Sprax Lines,  July 2010

#include "TrieNode.hpp"
#include "CharMap.hpp"


TrieNode::TrieNode(const CharMap &charMap, TrieNode *parent, int depth) 
    : firstBranch(NULL), nextBranch(NULL), parent(parent), mWord(NULL), mStem(NULL)
    , mDepth(depth), /*numStemWords(0),*/ firstWordNode(NULL), nextStemNode(NULL)
    //, nextWordNode(NULL)
{
    mBranches = (TrieNode**)calloc(charMap.targetSize(), sizeof(TrieNode*)) - charMap.targetOffset();
};

void TrieNode::printWordsAll() const
{
    if (mWord != NULL) {
        printf("%s\n", mWord);
    }
    for (const TrieNode *node = firstBranch; node != NULL; node = node->nextBranch) {
        node->printWordsAll();
    }
}

