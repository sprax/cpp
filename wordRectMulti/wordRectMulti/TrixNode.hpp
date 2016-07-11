// TrixNode.hpp : trie nodes for word tries, 
// where allowed chars are all contiguous.
// See TrixNode.hpp for nodes that can use a 
// variable number of non-contiguous chars.
// 
// Sprax Lines,  July 2010

#ifndef TrixNode_hpp
#define TrixNode_hpp

#include "TrieNode.hpp"

class TrixNode : TrieNode
{
	static const CharMap &sCharMap;

protected:

	////TrixNode*  parent;          // One level up in the trie tree is the node representing this stem's "prefix" (the string w/o last this one's last letter)
	////const int  mDepth;			// level in the tree.  Note that stem[depth] is this node's letter.  This const int prevents generation of assignment operator (ok).

	////TrixNode  **mBranches;                   // all possible continuations...
	////const TrixNode *firstBranch;// First non-NULL branch is the first node in the mBranches as a linked list
	////const TrixNode *nextBranch;	// Assumes fixed length; otherwise, the next branch depends on the desired word length
    ////TrixNode *firstWordNode;	// node of first word that completes this one's stem (this, if this is a word node)
    ////TrixNode *nextStemNode;		// node of first stem after this one 
    //  TrixNode * nextWordNode;	// node of first word that does not start with this one's stem.  Examples: 
    // For stem a, first word is aah, next stem is b, and next word is baa (not aba or aal); 
    // For ac, first word is ace, next stem is ad, and next word is add (not act);
    // For stem ace, first word is ace (itself), and the next stem & next word are the same: act.
    // nextStemNode is more useful than nextWordNode.

	////const char *mWord;  // The word, if this is a word node; NULL, if it is not a word node.  Used as a boolean and for printing.
	////const char *mStem;  // Some word that completes this stem; only the first N chars matter, where N == depth.

public:
    TrixNode(const CharMap &charMap, TrixNode *parent, int depth) 
		: TrieNode(charMap, parent, depth)
	////, parent(parent), mDepth(depth)
    ////, firstBranch(NULL), nextBranch(NULL)
	////, firstWordNode(NULL), nextStemNode(NULL) //, nextWordNode(NULL)
	////, mWord(NULL), mStem(NULL)
    {
		// Subtract sFirstLetter from address to use letters directly as indices.
        // mBranches = (TrixNode**)calloc(mCharMap.getRangeCount(), sizeof(TrixNode*));
    };
    TrixNode (const TrixNode&);                                                 // Do not define.
    TrixNode& operator=(const TrixNode&);                                       // Do not define.
    ~TrixNode() {
		delete mBranches;
	}

////inline const char     * getWord()			const { return mWord; }			// NULL, if this is not a word-node
////inline const char     * getFirstWord()	    const { assert(firstWordNode); return firstWordNode->getWord(); }
////inline const char     * getStem()           const { return mStem; }
////inline       int        getDepth()			const { return mDepth; }
////inline		 int	    getLetter()			const { return mStem[mDepth]; }
////inline		 int	    getLetterAt(int ix) const { return mStem[ix]; }
    inline		 TrixNode * getParent()			const { return (TrixNode*)parent; }		
////inline const TrixNode * getFirstChild()	    const { return firstBranch; }	
////inline const TrixNode * getNextBranch()	    const { return nextBranch; }	
////inline const TrixNode * getFirstWordNode()	const { return firstWordNode; }	
////inline const TrixNode * getNextStemNode()   const {	return nextStemNode; }
    //	inline const TrixNode * getNextWordNode()	const { return nextWordNode; }	// nextWordNode == nextStemNode->firstWordNode, so we don't really need nextWordNode

	bool isSubKeyStem(const char *subKey, int subKeyLen, const TrieNode* lastStemNode) const;	// deprecated: flexible but tricky to call

    /*****************************************
    template <typename T>
    friend class WordTrie;
    
    template <typename T>
	int  testTrixNode(const WordTrie<T> &trie, const CharMap &charMap);
    *****************************************/
};

#endif // TrixNode_hpp