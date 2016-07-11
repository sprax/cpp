// TrieNode.cpp : trie nodes for words

#include "TrieNode.hpp"


bool TrieNode::isSubKeyStem(const char *subKey, int subKeyLen, const TrieNode* lastStemNode) const	// deprecated: flexible but tricky to call
{
    //  subKey may be the address of a letter in the interior of a key; if so, 'this' must correspond to the letter at that address.
    //  Follow the branches for each letter in subKey, starting from this, 
    //      return true if key is a valid word stem, or false if it is not, 
    //      and set lastStemNode to be the node of the last letter that is part of a valid stem.

    // assert(subKey && *subKey == stem[depth]);		// The letter starting subKey must match letter at this node.
    const TrieNode * node = lastStemNode = this;
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

TrieNode* TrieNode::subTrix(const char *key, int idx)		// deprecated
{
    //  Follow the branches for each letter in key, and:
    //      return the last node, if key is a valid word stem, or 
    //      return NULL, if key is not a stem for any word in the trie.
    TrieNode *node = this;
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

TrieNode* TrieNode::subTrie(const char *nullTerminatedKey)	// deprecated: needs null-termination, loses info.
{
    //  Follow the branches for each letter in key, and:
    //      return the last node, if key is a valid word stem, or 
    //      return NULL, if key is not a stem for any word in the trie.
    //  The key must be null-terminated, and we throw away the information of where the key fails.
    TrieNode *node = this;
    for (const char *p = nullTerminatedKey; *p != NULL; p++) {
        node = node->branches[*p - firstLetter];
        if (node == NULL) {
            return  NULL;			// Not found - reached end of branch
        }
    }
    return node;
}

TrieNode * TrieNode::insertWord(const char *word, TrieNode *prevWordNode)
{
    // Traverse down the trie until we reach the end of the word, creating nodes as necessary
    assert(word != NULL);
    TrieNode *node = this;
    for (const char *pc = word; ; ) {
        if (*pc == NULL) {			  // End of the word.
            node->word = word;        // Don't _strdup or delete this word; its memory is owned by main & shared by others.
            // For all ancestors not already pointing to an earlier word, set this node as the first completion of their stem:
#ifdef _DEBUG
            if (node->depth == 4)
                int debug = node->depth;
#endif

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
                        parent->stem          =  word;
                    } else {
                        break;				// all higher (shallower) nodes already point to words that precede this one
                    }
                }
            }
            return node;
        }
        int ic = *pc++ - firstLetter;
        assert(0 <= ic && ic < TrieNode::numLetters);
        if (node->branches[ic] == NULL) {
            node->branches[ic] =  new TrieNode(node, node->depth + 1);   // create new branch node as a child with a backpointer to its parent.
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
        if (node->stem == NULL)
            node->stem =  word;			// pointer to the first or last word off this stem, it doesn't matter, they're same, up to depth characters
    //  node->numStemWords++;           // increment the number of words stemmed through this node?  We don't need to know.
    }
    //return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
}

void TrieNode::printWordsAll() const
{
    if (word != NULL) {
        printf("%s\n", word);
    }
#if USE_LAST
    for (const TrieNode *node = firstBranch; node != NULL; node = node->nextBranch) {
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

void TrieNode::printNextWordMemo()
{
    const TrieNode *next = getFirstWordNode();
    if (next != NULL) {
        printf("%s\n", next->word);
    }
}

void TrieNode::printWordsOfLength(int length) const
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

void TrieNode::printNextTree() const
{
    const TrieNode *node = getFirstWordNode();
    if (node != NULL) {
        assert(node->word != NULL);
        printf("%s\n", node->word);
    }
}
