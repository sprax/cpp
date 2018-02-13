// TrieNode.hpp : trie nodes for word tries
// Sprax Lines,  July 2010

// TODO: Could probably eliminate mWord and just keep mStem.  Then the node is a word-node iff depth == length...

#ifndef TrieNode_hpp
#define TrieNode_hpp

#include <stdio.h>
#include <map>

#ifdef _DEBUG
#include <assert.h>
#else
#define NDEBUG          1
#ifndef assert
#define assert(e_x_p_r)  ((void)0)
#endif
#endif

const int MAXLEN  = 28;			// Maximum word length
const int BUFSIZE = MAXLEN + 2; // extra room for terminating NULL, sentinel, or root node at index -1

struct ltstr
{
    inline bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) < 0;
    }
};
typedef std::map<const char*, int, ltstr> WordMap;

#define USE_CALLOC 1

class TrieNode		// final, for now.  No destructor needed, for now.
{
protected:

    static const int sFirstLetter = 'a';
    static const int sLastLetter  = 'z';
    static const int sNumLetters  =  1 + sLastLetter - sFirstLetter;

#if USE_CALLOC
    TrieNode  **mBranches;                   // all possible continuations...
#else
    TrieNode   *mBranchesMem[sNumLetters];   // all possible continuations...
    TrieNode  **mBranches;                   // all possible continuations...
#endif

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
    TrieNode(TrieNode *parent, int depth) : firstBranch(NULL), nextBranch(NULL), parent(parent), mWord(NULL), mStem(NULL)
        , mDepth(depth), /*numStemWords(0),*/ firstWordNode(NULL), nextStemNode(NULL) //, nextWordNode(NULL)
    {
#if USE_CALLOC
        mBranches = (TrieNode**)calloc(sNumLetters, sizeof(TrieNode*)) - sFirstLetter;
#else
        memset(mBranchesMem, 0, sNumLetters*sizeof(TrieNode*));
        mBranches = mBranchesMem - sFirstLetter;
#endif
    };
    TrieNode (const TrieNode&);                                                 // Do not define.
    TrieNode& operator=(const TrieNode&);                                       // Do not define.
    ~TrieNode() {}				 												// This class owns nothing on the heap.

    inline const char     * getWord()			const { return mWord; }			// NULL, if this is not a word-node
    inline const char     * getFirstWord()	    const { assert(firstWordNode); return firstWordNode->getWord(); }
    inline const char     * getStem()           const { return mStem; }
    inline       int        getDepth()			const { return mDepth; }
    inline		 int	    getLetter()			const { return mStem[mDepth]; }
    inline		 int	    getLetterAt(int ix) const { assert(0 <= ix && ix < sNumLetters); return mStem[ix]; }
    inline		 TrieNode * getParent()			const { return parent; }		// NULL, if this is a root node
    inline const TrieNode * getFirstChild()	    const { return firstBranch; }	
    inline const TrieNode * getNextBranch()	    const { return nextBranch; }	
    inline const TrieNode * getFirstWordNode()	const { return firstWordNode; }	
    inline const TrieNode * getNextStemNode()   const {	return nextStemNode; }
    //	inline const TrieNode * getNextWordNode()	const { return nextWordNode; }	// nextWordNode == nextStemNode->firstWordNode, so we don't really need nextWordNode

    inline const TrieNode * getBranchAtLetter(int letter) const {
        //assert(sFirstLetter <= letter && letter <= sLastLetter);
        if (sFirstLetter <= letter && letter <= sLastLetter)
            return mBranches[letter];    // may be NULL 
        else
            return NULL;
    }
    const TrieNode * getNextBranchFromLetter(int letter) const {
        assert(mBranches[letter]);
        return mBranches[letter]->nextBranch; 
    }
    inline const TrieNode * getFirstWordNodeFromLetter(int ic) const {
        if (mBranches[ic] != NULL) {
            return mBranches[ic]->firstWordNode;
        }
        // Still here?  Expect few non-NULL mBranches, so use the links instead of the array indices
        for (const TrieNode *branch = firstBranch; branch != NULL; branch = branch->nextBranch) {
            if (branch->mStem[0] >= ic) {
                return branch->firstWordNode;
            }
        }
        return NULL;
    }

    inline bool hasWord(const char *key) const
    {
        // Traverse down the trie until the end of string is found
        const TrieNode *node = this;
        for (const char *p = key; *p != '\0'; p++) {
            node = node->mBranches[*p];
            if (node == NULL) {
                return  false;			// Not found - reached end of branch
            }
        }
        return (node->mWord != NULL);
    }

    TrieNode * insertWord(const char *word, TrieNode *prevWordNode);	// insert word and set prev word nodes' forward links

    const TrieNode* subTrie(const char *nullTerminatedKey)  const;  // deprecated: needs null-termination, does not .
    const TrieNode* subTrix(const char *key, int idx)       const;	// deprecated: loses info of where the key fails.
    bool isSubKeyStem(const char *subKey, int subKeyLen, const TrieNode* lastStemNode) const;	// deprecated: flexible but tricky to call
    void printWordsAll() const;
};

#endif // TrieNode_hpp
