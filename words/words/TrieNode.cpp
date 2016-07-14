// TrieNode.cpp : trie nodes for words
// Sprax Lines,  July 2010

#include "stdafx.h"
#include "TrieNode.hpp"


TrieNode * TrieNode::insertWord(const char *word, TrieNode *prevWordNode)
{
    // Traverse down the trie until we reach the end of the word, creating nodes as necessary.
    // When we get to the word's end, create forward links from previously created nodes to the
    // longest stem of this word that is new.  Optimizing the prefix tree with such links greatly
    // speeds up finding word rectangles or what not.  No common ancestor algorithm necessarys; 
    // just traverse (i.e. follow the added links).
    assert(word != NULL);
    TrieNode *node = this;
    for (const char *pc = word; ; ) {
        if (*pc == NULL) {			  // End of the word.
            node->mWord = word;        // Don't _strdup or delete this word; its memory is owned by main & shared by others.
            // For all ancestors not already pointing to an earlier word, set this node as the first completion of their stem:
            if (prevWordNode != NULL) {
                TrieNode *nodeParent = node, *prevParent = prevWordNode, *nextStem = node;
                // set next word and next stem for shallower nodes that don't already have these pointers set.
                while (nodeParent->firstWordNode == NULL) {
                    nodeParent->firstWordNode =  node;
                    nextStem = nodeParent;
                    nodeParent = nodeParent->getParent();
                    if (nodeParent == NULL) {
                        break;				
                    }
                    prevParent = prevParent->getParent();
                }
                for (TrieNode *pwn = prevWordNode; pwn != prevParent; pwn = pwn->getParent()) {
                    assert(pwn != NULL);
                    pwn->nextStemNode =  nextStem;
                }
            } else {
                for (TrieNode *parent = node; parent != NULL; parent = parent->getParent()) {
                    if (parent->firstWordNode == NULL) {
                        parent->firstWordNode =  node;
                        parent->mStem         =  word;
                    } else {
                        break;				// all higher (shallower) nodes already point to words that precede this one
                    }
                }
            }
            return node;
        }
        int ic = *pc++;
        assert('a' <= ic && ic <= 'z');
        if (node->mBranches[ic] == NULL) {
            node->mBranches[ic] =  new TrieNode(node, node->mDepth + 1);   // create new branch node as a child with a backpointer to its parent.
            // Add new node to linked list using iteration (we save memory by not storing a lastBranch)
            if (node->firstBranch == NULL) {
                node->firstBranch = node->mBranches[ic];
            } else for (int ib = ic; --ib >= 0; ) {
                if (node->mBranches[ib] != NULL) {
                    node->mBranches[ib]->nextBranch = node->mBranches[ic];
                    break;
                }
            }
        }
        node = node->mBranches[ic];
        if (node->mStem == NULL)
            node->mStem =  word;			// pointer to the first or last word off this stem, it doesn't matter, they're same, up to depth characters
    //  node->numStemWords++;           // increment the number of words stemmed through this node?  We don't need to know.
    }
    //  return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
}


bool TrieNode::isSubKeyStem(const char *subKey, int subKeyLen, const TrieNode* lastStemNode) const	// deprecated: flexible but tricky to call
{
    //  subKey may be the address of a letter in the interior of a key; if so, 'this' must correspond to the letter at that address.
    //  Follow the mBranches for each letter in subKey, starting from this, 
    //      return true if key is a valid word stem, or false if it is not, 
    //      and set lastStemNode to be the node of the last letter that is part of a valid stem.

    assert(subKey && *subKey == mStem[mDepth]);		// The letter starting subKey must match letter at this node.
    const TrieNode * node = lastStemNode = this;
    while (subKeyLen >= 0) {
        node = node->mBranches[*subKey++];
        if ( ! node ) {
            return false;
        }
        lastStemNode = node;
        --subKeyLen;
    }
    return true;
}

const TrieNode* TrieNode::subTrix(const char *key, int idx) const		// deprecated, doesn't say where the key failed
{
    //  Follow the mBranches for each letter in the key up the specified index, and:
    //      return the last node, if key is a valid word stem, or 
    //      return NULL, if key is not a stem for any word in the trie.
    const TrieNode *node = this;
    do {
        node = node->mBranches[*key++];
    } while (node != NULL && --idx >= 0);
    return node;
}

const TrieNode* TrieNode::subTrie(const char *nullTerminatedKey) const	// deprecated: depends on null-termination, loses info.
{
    //  Follow the mBranches for each letter in key, and:
    //      return the last node, if key is a valid word stem, or 
    //      return NULL, if key is not a stem for any word in the trie.
    //  The key must be null-terminated, and we throw away the information of where the key fails.
    const TrieNode *node = this;
    for (const char *p = nullTerminatedKey; *p != NULL; p++) {
        node = node->mBranches[*p];
        if (node == NULL) {
            return  NULL;			// Not found - reached end of branch
        }
    }
    return node;
}

void TrieNode::printWordsAll() const
{
    if (mWord != NULL) {
        printf("%s\n", mWord);
    }
    for (const TrieNode *node = firstBranch; node != NULL; node = node->nextBranch) {
        node->printWordsAll();
    }
}

