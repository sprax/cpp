// WordNode.hpp : trie nodes for word tries
// Sprax Lines,  July 2010

//  TODO:   Rename WordNode to RectNode
//          Could probably eliminate mWord and just keep mStem.  Then the node is a word-node iff depth == length...

#ifndef WordNode_hpp
#define WordNode_hpp

#include "stdafx.h"
#ifndef WIN32
#include <assert.h>
//#include <map>
#endif

#include "TrieNode.hpp"

class WordNode : public TrieNode
{
protected:
    WordNode       * mParent;       // One level up in the trie tree is the node representing this stem's "prefix" (the string w/o last this one's last letter)
    const WordNode * mFirstWordNode;// node of first word that completes this one's stem (this, if this is a word node)
    const WordNode * mNextStemNode; // node of first stem after this one 
//  WordNode * mNextWordNode;	    // node of first word that does not start with this one's stem.  Examples: 
                                    // For stem a, first word is aah, next stem is b, and next word is baa (not aba or aal); 
                                    // For ac, first word is ace, next stem is ad, and next word is add (not act);
                                    // For stem ace, first word is ace (itself), and the next stem & next word are the same: act.
                                    // mNextStemNode is more useful than nextWordNode.
    int mTextCount;                 // number of words that include this node in their stems    // We don't use it.
    int mWordCount;                 // NB: SUM{all branches' text counts} == mTextCount - mWordCount
        

public:
#if STEM_IS_REF
    WordNode(WordNode *parent, WordLenT depth, const char *&stem) : TrieNode(depth, stem), mParent(parent), mFirstWordNode(NULL), mNextStemNode(NULL)
        , mTextCount(0), mWordCount(0)  {};
#else
    WordNode(WordNode *parent, WordLenT depth) : TrieNode(depth), mParent(parent), mFirstWordNode(NULL), mNextStemNode(NULL)
        , mTextCount(0), mWordCount(0)  {};
#endif
    

protected:
    WordNode (const WordNode&);                     // Do not define
    WordNode& operator=(const WordNode&);			// Do not define.
   ~WordNode() {}								    // This class owns nothing on the heap.

    inline void               setStem(const char *stem)  { mStem = stem; }	
    inline void               setFirstWordNode(const WordNode *node)  { mFirstWordNode = node; }	
    inline void               setNextStemNode(const WordNode *node)   {	mNextStemNode = node; }
    inline void               setNextBranch(const WordNode *node)   {	mNextBranch = node; }
    
public:
    inline virtual WordNode * getParent()		    const { return mParent; }		// NULL, if this is a root node.  // TODO: make non-virtual?
    inline const   WordNode * getFirstChild()	    const { return static_cast<const WordNode*>(mFirstChild); }	
    inline const   WordNode * getNextSibling()	    const { return static_cast<const WordNode*>(mNextBranch); }	
    inline const   WordNode * getFirstWordNode()	const { return mFirstWordNode; }	
    inline const   WordNode * getNextStemNode()     const {	return mNextStemNode; }
    inline const   WordNode * getNextWordNode()	    const { return mNextStemNode->mFirstWordNode; }
    inline const   char     * getWord()	            const { return mWord; }			// NULL, if this is not a word-node
    inline const   char     * getFirstWord()	    const { assert(mFirstWordNode); return mFirstWordNode->getWord(); }	// NULL, if this is not a word-node
    inline         int        getDepth()			const { return mDepth; }
    inline		   int	      getLetter()			const { return mStem[mDepth]; }
    inline		   int	      getLetterAt(int ix)   const { assert(0 <= ix && ix < sNumLetters); return mStem[ix]; }
    inline         WordNode * getBranchAtIndex(int index) const {
        return static_cast<WordNode *>(TrieNode::getBranchAtIndex(index));                        // may return NULL 
    }
    inline const   WordNode * getBranchAtLetter(int letter) const {
        return static_cast<const WordNode *>(TrieNode::getBranchAtLetter(letter));                      // may return NULL 
    }
    inline const   WordNode * getNextBranchFromLetter(int letter) const {
        return static_cast<const WordNode *>(TrieNode::getNextBranchFromLetter(letter));                // may return NULL 
    }


public: 
    inline const WordNode * getFirstWordNodeFromLetter(int ic) const {
        if (mBranches[ic - sFirstLetter] != NULL) {
            return static_cast<const WordNode*>(mBranches[ic - sFirstLetter])->mFirstWordNode;
        }
        // Still here?  Expect few non-NULL mBranches, so use the links instead of the array indices
        for (const WordNode *branch = getFirstChild(); branch != NULL; branch = branch->getNextSibling() ) {
            if (branch->mStem[0] >= ic) {
                return branch->mFirstWordNode;
            }
        }
        return NULL;
    }

    // insert word and set prev word node's forward links
    virtual WordNode * addWord(const char *word, WordLenT wordLen, WordNode *prevWordNode);

    const WordNode* subTrie(const char *nullTerminatedKey)  const;  // deprecated: needs null-termination, does not tell where key fails.
    bool isSubKeyStem(const char *subKey, int subKeyLen, const WordNode* lastStemNode) const;	// deprecated: flexible but tricky to call
    void printWordsAll() const;
};

#endif // WordNode_hpp