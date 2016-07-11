// WordTrie.hpp : word prefix tree with forward links (optimized for speed at the expense of memory)
// Sprax Lines, July 2010

#ifndef WordTrie_hpp
#define WordTrie_hpp

#include "CharFreqMap.hpp"
#include "TrieNode.hpp"


template <typename T>
class WordTrie 
{
public:

	WordTrie(const T &charMap, int wordLen) : mCharMap(charMap), mWordLength(wordLen)  
    {
        mRoot = new TrieNode(mCharMap, NULL, 0);
    }
    WordTrie(WordTrie &);						// Prevent pass-by-value by not defining copy constructor.
    WordTrie& operator=(const WordTrie&);		// Prevent assignment by not defining this operator.
    ~WordTrie() { }                             // Destroy all the nodes?	TODO: depth-first infanticide...
   
    inline       uint       charIndex(uchr uc)		  const { return mCharMap.charToIndex(uc); }
    inline const T&         charMap()                 const { return mCharMap; }
    inline const TrieNode * getRoot()                 const { return mRoot; }
    inline       int        getWordLength()           const { return mWordLength; }
    inline const TrieNode * getFirstWordNode()        const { return mRoot->getFirstWordNode(); }
    inline const TrieNode * getFirstWordNodeFromIndex(int ix)   const { return mRoot->getFirstWordNodeFromIndex(ix); }
    inline bool             hasWord(const char *key)  const { return containsWord(key) != NULL; }
    void printWordsAll()			                  const { mRoot->printWordsAll(); }

    TrieNode * insertWord(const char *word, TrieNode *prevWordNode)
    {
        // Traverse down the trie until we reach the end of the word, creating nodes as necessary.
        // When we get to the word's end, create forward links from previously created nodes to the
        // longest stem of this word that is new.  Optimizing the prefix tree with such links greatly
        // speeds up finding word rectangles or what not.  No common ancestor algorithm necessary; 
        // just traverse (i.e. follow the added links).
        assert(word != NULL);
#ifdef _DEBUG
        if (prevWordNode != NULL) {
            unsigned int prev0 = prevWordNode->getStem()[0];
            unsigned int word0 = word[0];
            assert(prev0 <= word0);
        }
#endif
        TrieNode *node = mRoot;
        for (const char *pc = word; ; ) {
            if (*pc == '\0') {			  // End of the word.
                // We have reached the end of a word, so finish up this node
                // as a word node and set some "shortcut" pointers link it
                // or its antecedents to other nodes.  NB: These shortcuts
                // may depend on the words being added in a canonical order,
                // as in strictly sorted order.
                // First set this word node's word:
                // Don't _strdup or delete this word; its memory is owned 
                // by the caller (wordRectSearchMgr) & shared by others.
                node->mWord = word;       
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
            uchr ucx = *pc++;
            assert(mCharMap.sourceMinChar() <= ucx && ucx <= mCharMap.sourceMaxChar());
			uint ix = charIndex(ucx);
            if (node->mBranches[ix] == NULL) {
                node->mBranches[ix] =  new TrieNode(mCharMap, node, node->mDepth + 1);   // create new branch node as a child with a backpointer to its parent.
                // Add new node to linked list using iteration (we save memory by not storing a lastBranch)
                if (node->firstBranch == NULL) {
                    node->firstBranch = node->mBranches[ix];    // This depends on alphabetical ordering!
                } else for (int ib = ix; --ib >= 0; ) {
                    if (node->mBranches[ib] != NULL) {
                        node->mBranches[ib]->nextBranch = node->mBranches[ix];
                        break;
                    }
                }
            }
            node = node->mBranches[ix];
            if (node->mStem == NULL)
                node->mStem =  word;    // pointer to the first or last word off this stem, it doesn't matter, they're same, up to depth characters
            //  node->numStemWords++;   // increment the number of words stemmed through this node?  We don't need to know.
        }
        //  return NULL;					// unreachable code: either we return out of the loop or it or goes forever.
    }


	const TrieNode * containsWord(const char *key) const
	{
		// Traverse down the trie until we get to the end of the string:
		const TrieNode *node = mRoot;
		while (*key != NULL) {
			uchr ucx = *key++;
			uint uix = mCharMap.charIndex(ucx);
			node = node->mBranches[uix];
			if (node == NULL) {
				return  NULL;	// Not found - reached end of branch
			}
		}
		return node->mWord == NULL ? NULL : node;
	}



    const TrieNode* getNextWordNodeAndIndex(const TrieNode *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const // fromBeg can change
    {
        // We could memoize lazily using nextWordNode.  That is, either change this 
        // to start with the offending node and just set/get its nextWordNode (if the 
        // disqualified node is passed in!), or else replace the word member with a pointer to basically
        // an array of nextWordNodes for each length, so we just set/get/return nextWordNodes[fromBeg],
        // where fromBeg is rationalized to be in [0, wordLength).  But if we already have the offending
        // node, so we don't have to traverse (as below) to find it (or rather, its parent), then getting 
        // the next different stem is so simple that we don't really need to make a separate method for it.  
        // As it is, we don't memoize lazily, we just do it once and for all in insertWord.

        // Anyway, here is the mon-memoized code.  It was used as a reference implementation before optimizing
        // all needed traversal using links.  I think it's slower than link-following by a factor of 2 or so.

        // Get the parent of the node containing the first 'bad' letter (if we already know it, we don't need to use this method).
        const TrieNode *parent = NULL;
        if (fromBeg < fromEnd) {
            // Start at the root and find the starting parent by going deeper through children:
            parent = mRoot;
            for (int j = 0; j < fromBeg; j++) {
                parent = parent->getBranchAtIndex(mCharMap.charIndex(keyWord[j]));
            }
        } else {
            // Start at the word and find the starting parent by going up through parents:
            parent = keyNode;
            while (--fromEnd >= 0) {
                parent = parent->getParent();
            }
        }
        assert(parent != NULL);

        while (fromBeg >= 0) {
            const TrieNode *next = parent->getNextBranchFromIndex(mCharMap.charIndex(keyWord[fromBeg]));
            if (next != NULL) {
                return next->getFirstWordNode();
            }
            fromBeg--;
            parent = parent->getParent();
        }
        fromBeg = 0;
        return NULL;
    }


	const TrieNode* subTrix(const char *key, int idx) const		// deprecated, doesn't say where the key failed
	{
		//  Follow the mBranches for each letter in the key up the specified index, and:
		//      return the last node, if key is a valid word stem, or 
		//      return NULL, if key is not a stem for any word in the trie.
		const TrieNode *node = mRoot;
		do {
			uchr uc = *key++;
			uint ux = charIndex(uc);
			node = node->mBranches[ux];
		} while (node != NULL && --idx >= 0);
		return node;
	}

	const TrieNode* subTrie(const char *nullTerminatedKey) const	// deprecated: depends on null-termination, loses info.
	{
		//  Follow the mBranches for each letter in key, and:
		//      return the last node, if key is a valid word stem, or 
		//      return NULL, if key is not a stem for any word in the trie.
		//  The key must be null-terminated, and we throw away the information of where the key fails.
		const TrieNode *node = mRoot;
		for (const char *pc = nullTerminatedKey; *pc != NULL; pc++) {
			uchr uc = *pc;
			uint ux = mCharMap.charIndex(uc);
			node = node->mBranches[ux];
			if (node == NULL) {
				return  NULL;			// Not found - reached end of branch
			}
		}
		return node;
	}

	bool isSubKeyStem(const char *subKey, int subKeyLength, const TrieNode* lastStemNode) const	// deprecated: flexible but tricky to call
	{
		//  subKey may be the address of a letter in the interior of a key; if so, 'this' must correspond to the letter at that address.
		//  Follow the mBranches for each letter in subKey, starting from this, 
		//      return true if key is a valid word stem, or false if it is not, 
		//      and set lastStemNode to be the node of the last letter that is part of a valid stem.

		const TrieNode * node = lastStemNode;
		assert(subKey && *subKey == node->mStem[this->mDepth]);		// The letter starting subKey must match letter at this node.
		while (subKeyLength >= 0) {
			uchr uc = *subKey++;
			uint ux = charIndex(uc);
			node = node->mBranches[ux];
			if ( ! node ) {
				return false;
			}
			lastStemNode = node;
			--subKeyLength;
		}
		return true;
	}


private:
	const T		& mCharMap;
	const uint	  mWordLength;	// All words must be this long.
    TrieNode	* mRoot;        // The root node has no parent, and its depth == 0.
};


#endif // WordTrie_hpp