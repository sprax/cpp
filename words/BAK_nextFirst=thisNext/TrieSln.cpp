// TrieSln.cpp : trie nodes 

#include "stdafx.h"


bool TrieSln::isSubKeyStem(const char *subKey, int subKeyLen, const TrieSln* lastStemNode) const	// deprecated: flexible but tricky to call
{
    //  subKey may be the address of a letter in the interior of a key; if so, 'this' must correspond to the letter at that address.
    //  Follow the branches for each letter in subKey, starting from this, 
    //      return true if key is a valid word stem, or false if it is not, 
    //      and set lastStemNode to be the node of the last letter that is part of a valid stem.

    // assert(subKey && *subKey == stem[depth]);		// The letter starting subKey must match letter at this node.
    const TrieSln * node = lastStemNode = this;
    while (subKeyLen >= 0) {
        node = node->branches[*subKey++ - firstLetter];
        if ( ! node ) {
            return false;
        }
        lastStemNode = node;
        --subKeyLen;
    }
    return true;
}

TrieSln* TrieSln::subTrix(const char *key, int idx)		// deprecated
{
    //  Follow the branches for each letter in key, and:
    //      return the last node, if key is a valid word stem, or 
    //      return NULL, if key is not a stem for any word in the trie.
    TrieSln *node = this;
#if 0
    while (idx >= 0) {
        node = node->branches[*key++ - firstLetter];
        if ( ! node )
            return NULL;
        --idx;
    }
#else
    do {
        node = node->branches[*key++ - firstLetter];
    } while (node != NULL && --idx >= 0);
#endif
    return node;
}

TrieSln* TrieSln::subTrie(const char *nullTerminatedKey)	// deprecated: needs null-termination, loses info.
{
    //  Follow the branches for each letter in key, and:
    //      return the last node, if key is a valid word stem, or 
    //      return NULL, if key is not a stem for any word in the trie.
    //  The key must be null-terminated, and we throw away the information of where the key fails.
    TrieSln *node = this;
    for (const char *p = nullTerminatedKey; *p != NULL; p++) {
        node = node->branches[*p - firstLetter];
        if (node == NULL) {
            return  NULL;			// Not found - reached end of branch
        }
    }
    return node;
}

TrieSln * TrieSln::insertWord(const char *word, TrieSln *prevWordNode)
{
    // Traverse down the trie until we reach the end of the word, creating nodes as necessary
    assert(word != NULL);
    TrieSln *node = this;
    for (const char *pc = word; ; ) {
        if (*pc == NULL) {			  // End of the word.
            node->word = word;        // Don't _strdup or delete this word; its memory is owned by main & shared by others.
            // For all ancestors not already pointing to an earlier word, set this node as the first completion of their stem:
#ifdef _DEBUG
            if (node->depth == 4)
                int debug = node->depth;
#endif
            if (prevWordNode != NULL) {
                for (TrieSln *nodeP = node, *prevP = prevWordNode; nodeP != NULL; nodeP = nodeP->getParent(), prevP = prevP->getParent()) {
                    if (nodeP->firstWordNode == NULL) {
                        nodeP->firstWordNode =  node;
                        prevP->nextWordNode  =  node;
                        prevP->nextStemNode  =  nodeP;
                    } else {
                        break;				// all higher (shallower) nodes already point to words that precede this one
                    }
                }
            } else {
                for (TrieSln *parent = node; parent != NULL; parent = parent->getParent()) {
                    if (parent->firstWordNode == NULL) {
                        parent->firstWordNode =  node;
                    } else {
                        break;				// all higher (shallower) nodes already point to words that precede this one
                    }
                }
            }

            return node;
        }
        int ic = *pc++ - firstLetter;
        assert(0 <= ic && ic < TrieSln::numLetters);
        if (node->branches[ic] == NULL) {
            node->branches[ic] =  new TrieSln(node, node->depth + 1);   // create new branch node as a child with a backpointer to its parent.
#if USE_LAST	// add new node to linked list using lastBranch
            if (node->lastBranch == NULL) {
                node->firstBranch =  node->lastBranch = node->branches[ic];
            } else {
                node->lastBranch->nextBranch = node->branches[ic];
                node->lastBranch = node->branches[ic];
            }
#else	// // add new node to linked list using iteration
            if (node->firstBranch == NULL) {
                node->firstBranch = node->branches[ic];
            } else for (int ib = ic; --ib >= 0; ) {
                if (node->branches[ib] != NULL) {
                    node->branches[ib]->nextBranch = node->branches[ic];
                    break;
                }
            }
#endif
        }
        node = node->branches[ic];
        node->stem = word;			// we end up with a pointer to the last word off this stem, which is the same as any other, up to depth characters
        node->words++;              // increment the number of words stemmed through this node
    }
    //return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
}

void TrieSln::printWordsAll() const
{
    if (word != NULL) {
        printf("%s\n", word);
    }
#if USE_LAST
    for (const TrieSln *node = firstBranch; node != NULL; node = node->nextBranch) {
        node->printWordsAll();
    }
#else
    for (int j = 0; j < numLetters; j++) {
        if (branches[j] != NULL) {
            branches[j]->printWordsAll();
        }
    }
#endif
}

void TrieSln::printNextWordMemo()
{
    const TrieSln *next = getFirstWordNode();
    if (next != NULL) {
        printf("%s\n", next->word);
    }
}

void TrieSln::printWordsOfLength(int length) const
{
    if (length == 0 && word != NULL) {
        printf("%s\n", word);
    }
    if (--length >= 0) {
        for (int j = 0; j < numLetters; j++) {
            if (branches[j] != NULL) {
                branches[j]->printWordsOfLength(length);
            }
        }
    }
}

void TrieSln::printNextTree() const
{
    const TrieSln *node = getFirstWordNode();
    if (node != NULL) {
        assert(node->word != NULL);
        printf("%s\n", node->word);
    }
}
