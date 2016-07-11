// WordNode.cpp : trie nodes for words, with forward links to the next different stem
// A trie made of these nodes could be good for finding word rectangles, 
// creating or solving crossword puzzles, etc.
// Sprax Lines,  July 2010

#include "stdafx.h"
#include "WordNode.hpp"
#include "WordMap.hpp"

WordNode * WordNode::addWord(const char *word, WordLenT, WordNode *prevWordNode)
{
    // Traverse down the trie until we reach the end of the word, creating nodes as necessary.
    // When we get to the word's end, create forward links from previously created nodes to the
    // longest stem of this word that is new.  Optimizing the prefix tree with such links greatly
    // speeds up finding word rectangles or what not.  No common ancestor algorithm necessarys; 
    // just traverse (i.e. follow the added links).
    assert(word != NULL);

    WordNode *node = this;
    for (const char *pc = word; ; ) {
        if (*pc == NULL) {          // End of the word.
            node->setWord(word);     // Don't _strdup or delete this word; its memory is owned by main & shared by others.
            // For all ancestors not already pointing to an earlier word, set this node as the first completion of their stem:
            if (prevWordNode != NULL) {
                WordNode *nodeParent = node, *prevParent = prevWordNode, *nextStem = node;
                // set next word and next stem for shallower nodes that don't already have these pointers set.
                while (nodeParent->mFirstWordNode == NULL) {
                    nodeParent->mFirstWordNode =  node;
                    nextStem = nodeParent;
                    nodeParent = nodeParent->getParent();
                    if (nodeParent == NULL) {
                        break;				
                    }
                    prevParent = prevParent->getParent();
                }
                for (WordNode *pwn = prevWordNode; pwn != prevParent; pwn = pwn->getParent()) {
                    assert(pwn != NULL);
                    pwn->mNextStemNode =  nextStem;
                }
            } else {
                for (WordNode *parent = node; parent != NULL; parent = parent->getParent()) {
                    if (parent->mFirstWordNode == NULL) {
                        parent->mFirstWordNode =  node;
                        parent->mStem         =  word;
                    } else {
                        break;				// all higher (shallower) nodes already point to words that precede this one
                    }
                }
            }
            return node;
        }
        int ic = *pc++ - sFirstLetter;
        assert(0 <= ic && ic < sNumLetters);
        if (node->mBranches[ic] == NULL) {
            node->mBranches[ic] =  new WordNode(node, node->mDepth + 1, word);   // create new branch node as a child with a backpointer to its parent.
            // Add new node to linked list using iteration (we save memory by not storing a lastBranch)
            if (node->mFirstChild == NULL) {
                node->mFirstChild = node->mBranches[ic];
            } else for (int ib = ic; --ib >= 0; ) {
                WordNode *lastBranch  = node->getBranchAtIndex(ib);
                if (lastBranch != NULL) {
                    lastBranch->setNextBranch(node->getBranchAtIndex(ic));
                    break;
                }
            }
        }
        node = node->getBranchAtIndex(ic);
        if (node->mStem == NULL)
            node->mStem =  word;			// pointer to the first or last word off this stem, it doesn't matter, they're same, up to depth characters
    //  node->numStemWords++;           // increment the number of words stemmed through this node?  We don't need to know.
    }
    //  return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
}


