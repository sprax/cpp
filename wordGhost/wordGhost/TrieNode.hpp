// TrieNode.hpp : trie nodes for word tries
// Sprax Lines,  July 2010

// TODO: Could probably eliminate mWord and just keep mStem.  Then the node is a word-node iff depth == length...

#ifndef TrieNode_hpp
#define TrieNode_hpp

#include "stdafx.h"
#ifndef WIN32
#include <stdio.h>
#include <map>
#ifdef _DEBUG
#include <assert.h>
#else
#define NDEBUG          1
#define assert(_expr_)  ((void)0)
#endif
#endif

const int MAXLEN  = 28;			// Maximum word length // TODO: eliminate this assumption
const int BUFSIZE = MAXLEN + 2; // extra room for terminating NULL, sentinel, or root node at index -1

class TrieNode
{
public:
    static const int sFirstLetter = 'a';
    static const int sLastLetter  = 'z';
    static const int sNumLetters  =  1 + sLastLetter - sFirstLetter;
    static const char *  sEmptyStr;

protected:
    TrieNode       * mBranches[sNumLetters]; // all possible continuations...
    const TrieNode * mFirstChild;            // First non-NULL branch is the first node in the mBranches as a linked list
    const TrieNode * mNextBranch;            // Assumes fixed length; otherwise, the next branch depends on the desired word length

    const WordLenT   mDepth; // level in the tree.  Note that stem[depth-1] is this node's letter.  This const int prevents generation of assignment operator (ok).
    const char     * mWord;  // The word, if this is a word node; NULL, if it is not a word node.  Used as a boolean and for printing.
#define STEM_IS_REF 1
#if     STEM_IS_REF
    const char     *&mStem;  // Some word that completes this stem; only the first N chars matter, where N == depth.
#else
    const char     * mStem;  // Some word that completes this stem; only the first N chars matter, where N == depth.
#endif

public:
#if STEM_IS_REF
    TrieNode(WordLenT depth, const char *&stem) : mFirstChild(NULL), mNextBranch(NULL), mDepth(depth)
        , mWord(NULL), mStem(stem)
    {
        memset(mBranches, 0, sNumLetters*sizeof(TrieNode*));
    };
#else
    TrieNode(WordLenT depth) : mFirstChild(NULL), mNextBranch(NULL), mDepth(depth), mWord(NULL), mStem(NULL)
    {
        memset(mBranches, 0, sNumLetters*sizeof(TrieNode*));
    };
#endif

protected:
    TrieNode (const TrieNode&);                     // Do not define
    TrieNode& operator=(const TrieNode&);			// Do not define.
   ~TrieNode() {}                                   // This class owns nothing on the heap.

    inline       void       setNextBranch(const TrieNode *node) { mNextBranch = node; }
    inline       void       setWord(const char * word)           { mWord = word; }
    inline       void       setStem(const char *&stem)           { mStem = stem; }	

public:
    inline const TrieNode * getFirstChild()	    const { return mFirstChild; }
    inline const TrieNode * getNextSibling()	const { return mNextBranch; }	// TODO: templatize ??
    inline const char     * getStem()	        const { return mStem; }			// First word containing this stem
    inline const char     * getWord()	        const { return mWord; }			// NULL, if this is not a word-node
    inline       bool       isWord()	        const { return mWord != NULL; } // true iff this is a word-node
    inline       WordLenT   getDepth()			const { return mDepth; }
    inline		 char	    getLetter()			const { return mStem[mDepth-1]; }
    inline		 int	    getLetterAt(int ix) const { assert(0 <= ix && ix < sNumLetters); return mStem[ix]; }

    inline       TrieNode * getBranchAtIndex(int index) const {
        assert(0 <= index && index <= sNumLetters);
        return mBranches[index];                                     // may return NULL 
    }
    inline const TrieNode * getBranchAtLetter(int letter) const {
        assert(sFirstLetter <= letter && letter <= sLastLetter);
        return mBranches[letter - sFirstLetter];                     // may return NULL 
    }
    const        TrieNode * getNextBranchFromLetter(int letter) const {
        assert(mBranches[letter - sFirstLetter]);
        return mBranches[letter - sFirstLetter]->mNextBranch; 
    }

    bool               hasWord(const char *key) const;
    virtual TrieNode * addWord(const char *word, WordLenT wordLen, TrieNode *prevWordNode);  // TODO: not all subclasses use all formal params - fixme?
    const TrieNode* subTrie(const char *nullTerminatedKey)  const;  // deprecated: needs null-termination, does not tell where key fails.
    const TrieNode* subTrix(const char *key, int idx)       const;	// deprecated: loses info of where the key fails.
    bool            isSubKeyStem(const char *subKey, int subKeyLen, const TrieNode* lastStemNode) const;	// deprecated: flexible but tricky to call
    void            printWords() const;
};

#endif // TrieNode_hpp