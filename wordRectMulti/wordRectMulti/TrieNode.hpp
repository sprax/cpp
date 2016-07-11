// TrieNode.hpp : trie nodes for word tries, 
// where allowed chars are all contiguous.
// See TrixNode.hpp for nodes that can use a 
// variable number of non-contiguous chars.
// 
// Sprax Lines,  July 2010

// TODO: Could probably eliminate mWord and just keep mStem.  Then the node is a word-node iff depth == length...

#ifndef TrieNode_hpp
#define TrieNode_hpp

#include <stdio.h>
#include <map>

#include "wordTypes.h"

class CharMap;      // forward declaration

class TrieNode		// final, for now.  No destructor needed, for now.
{
    template <typename T>
    friend class WordTrie;

protected:

    TrieNode  **mBranches;                   // all possible continuations...

    const TrieNode* firstBranch;// First non-NULL branch is the first node in the mBranches as a linked list
    const TrieNode* nextBranch;	// Assumes fixed length; otherwise, the next branch depends on the desired word length
    TrieNode* parent;           // One level up in the trie tree is the node representing this stem's "prefix" (the string w/o last this one's last letter)

    const char *mWord;  // The word, if this is a word node; NULL, if it is not a word node.  Used as a boolean and for printing.
    const char *mStem;  // Some word that completes this stem; only the first N chars matter, where N == depth.
    const int  mDepth;  // level in the tree.  Note that stem[depth] is this node's letter.  This const int prevents generation of assignment operator (ok).
    //  int numStemWords;   // number of words that include this node in their stems    // We don't use it.
    TrieNode * firstWordNode;	// node of first word that completes this one's stem (this, if this is a word node)
    TrieNode * nextStemNode;	// node of first stem after this one 
    //  TrieNode * nextWordNode;	// node of first word that does not start with this one's stem.  Examples: 
    // For stem a, first word is aah, next stem is b, and next word is baa (not aba or aal); 
    // For ac, first word is ace, next stem is ad, and next word is add (not act);
    // For stem ace, first word is ace (itself), and the next stem & next word are the same: act.
    // nextStemNode is more useful than nextWordNode.

public:
    TrieNode(const CharMap &charMap, TrieNode *parent, int depth);
    TrieNode (const TrieNode&);                                                 // Do not define.
    TrieNode& operator=(const TrieNode&);                                       // Do not define.
   ~TrieNode() { delete mBranches; }

    inline const char     * getWord()			const { return mWord; }			// NULL, if this is not a word-node
    inline const char     * getFirstWord()	    const { assert(firstWordNode); return firstWordNode->getWord(); }
    inline const char     * getStem()           const { return mStem; }
    inline       uint       getDepth()			const { return mDepth; }
    inline		 uchr	    getLetter()			const { return mStem[mDepth]; }
    inline		 uchr	    getLetterAt(int ix) const { return mStem[ix]; }
    inline		 TrieNode * getParent()			const { return parent; }		// NULL, if this is a root node
    inline const TrieNode * getFirstChild()	    const { return firstBranch; }	
    inline const TrieNode * getNextBranch()	    const { return nextBranch; }	
    inline const TrieNode * getFirstWordNode()	const { return firstWordNode; }	
    inline const TrieNode * getNextStemNode()   const {	return nextStemNode; }
    //	inline const TrieNode * getNextWordNode()	const { return nextWordNode; }	// nextWordNode == nextStemNode->firstWordNode, so we don't really need nextWordNode

    inline const TrieNode * getBranchAtIndex(int index) const {
        return mBranches[index];    // may return NULL 
    }
    const TrieNode * getNextBranchFromIndex(int index) const {
        assert(mBranches[index]);
        return mBranches[index]->nextBranch; 
    }
    inline const TrieNode * getFirstWordNodeFromIndex(int index) const {
        if (mBranches[index] != NULL) {
            return mBranches[index]->firstWordNode;
        }
        // Still here?  Expect few non-NULL mBranches, so use the links instead of the array indices
        for (const TrieNode *branch = firstBranch; branch != NULL; branch = branch->nextBranch) {
            if (branch->mStem[0] >= index) {
                return branch->firstWordNode;
            }
        }
        return NULL;
    }

    void printWordsAll() const;
};

#endif // TrieNode_hpp