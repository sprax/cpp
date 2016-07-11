#ifndef TrieNode_hpp
#define TrieNode_hpp

#include <stdio.h>
//#include <stdlib.h>
//#include <tchar.h>
#include <assert.h>
#include <time.h>
#include <signal.h>

//#include <string>
#include <iostream>
#include <map>
#include <hash_set>

#define USE_LAST 1

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

#define USE_BRANCHES_ON_STACK 1

class TrieNode		// final, for now.  No destructor needed, for now.
{
public:
    static const int firstLetter = 'a';
    static const int lastLetter  = 'z';
    static const int numLetters  =  1 + lastLetter - firstLetter;

protected:
#if USE_BRANCHES_ON_STACK
	TrieNode  *branches[numLetters]; // all possible continuations...
#else
    TrieNode **branches; // all possible continuations, or branches (call null continuations "buds", non-null ones "branches")
#endif

    const TrieNode* firstBranch;	// First non-NULL branch is the first node in the branches as a linked list
    const TrieNode* nextBranch;	// Assumes fixed length; otherwise, the next branch depends on the desired word length
    TrieNode* lastBranch;	    // Last non-NULL branch is the last node in the branches as a linked list (no sentinal; non-const because it changes during init)
	TrieNode* parent;            // One level up in the trie tree is the node representing this stem's "prefix" (the string w/o last this one's last letter)

    const char *word;   // The word, if this is a word node; NULL, if it is not a word node.  Used as a boolean and for printing.
    const char *stem;   // Some word that completes this stem; only the first N chars matter, where N == depth.
	const int  depth;   // level in the tree.  Note that stem[depth] is this node's letter.  This const int prevents generation of assignment operator (ok).
//  int numStemWords;   // number of words that include this node in their stems    // We don't use it.
    TrieNode * firstWordNode;	// node of first word that completes this one's stem (this, if this is a word node)
    TrieNode * nextStemNode;	    // node of first stem after this one 
//  TrieNode * nextWordNode;	    // node of first word that does not start with this one's stem.  Examples: 
								// For stem a, first word is aah, next stem is b, and next word is baa (not aba or aal); 
								// For ac, first word is ace, next stem is ad, and next word is add (not act);
								// For stem ace, first word is ace (itself), and the next stem & next word are the same: act.
								// nextStemNode is more useful than nextWordNode.
public:
    TrieNode(TrieNode *parent, int depth) : firstBranch(NULL), nextBranch(NULL), lastBranch(NULL), parent(parent)
		, word(NULL), stem(NULL), depth(depth), /*numStemWords(0),*/ firstWordNode(NULL), nextStemNode(NULL) //, nextWordNode(NULL)
    {
        // Beware of platform-dependent behavior performance: 
        // typedef int * pint;
        // int **arrayA = new pint[numLetters];  memset(arrayA, 0, numLetters*sizeof(int*));   // may reduce cache misses
        // int **arrayB = (pint*)calloc(numLetters, sizeof(int*));                             // may enable lazy memory mapping
#if USE_BRANCHES_ON_STACK
		memset(branches, 0, numLetters*sizeof(TrieNode*));
#else
        branches = static_cast<TrieNode**>(calloc(numLetters, sizeof(TrieNode*)));
#endif
    };
	~TrieNode() {}																// This class owns nothing on the heap.
	TrieNode& operator=(const TrieNode&);											// Do not define.

    inline const char    * getWord()			const { return word; }			// NULL, if this is not a word-node
	inline const char     * getFirstWord()	    const { assert(firstWordNode); return firstWordNode->getWord(); }	// NULL, if this is not a word-node
    inline       int       getDepth()			const { return depth; }
    inline		 int	   getLetter()			const { return stem[depth]; }
    inline		 int	   getLetterAt(int idx) const { assert(0 <= idx && idx < numLetters); return stem[idx]; }
    inline		 TrieNode * getParent()			const { return parent; }		// NULL, if this is a root node
    inline const TrieNode * getFirstBranch()	    const { return firstBranch; }	
    inline const TrieNode * getFirstWordNode()	const { return firstWordNode; }	
    inline const TrieNode * getNextStemNode()    const {	return nextStemNode; }
//	inline const TrieNode * getNextWordNode()	const { return nextWordNode; }	// nextWordNode == nextStemNode->firstWordNode, so we don't really need nextWordNode

    inline const TrieNode * getBranchAtLetter(int letter) const {
		assert(0 <= firstLetter && letter <= lastLetter);
		return branches[letter - firstLetter];    // may be NULL 
	}
	const TrieNode * getNextBranchFromLetter(int letter) const {
		assert(branches[letter - firstLetter]);
		return branches[letter - firstLetter]->nextBranch; 
	}
	inline const TrieNode * getFirstWordNodeFromLetter(int ic) const {
		if (branches[ic - firstLetter] != NULL) {
			return branches[ic - firstLetter]->firstWordNode;
		}
        // Still here?  Expect few non-NULL branch, so use the links instead of the array indices
		for (const TrieNode *branch = firstBranch; branch != NULL; branch = branch->nextBranch) {
			if (branch->stem[0] >= ic) {
				return branch->firstWordNode;
			}
		}
		return NULL;
	}

    inline bool hasWord(const char *key) const
    {
        // Traverse down the trie until the end of string is found
        const TrieNode *node = this;
        for (const char *p = key; *p != NULL; p++) {
            node = node->branches[*p - firstLetter];
            if (node == NULL) {
                return  false;			// Not found - reached end of branch
            }
        }
        return (node->word != NULL);
    }

    TrieNode* subTrie(const char *nullTerminatedKey);	// deprecated: needs null-termination, loses info.
	TrieNode* subTrix(const char *key, int idx);			// deprecated: loses info of where the key fails.
    bool isSubKeyStem(const char *subKey, int subKeyLen, const TrieNode* lastStemNode) const;	// deprecated: flexible but tricky to call

	TrieNode * insertWord(const char *word, TrieNode *prevWordNode);	// insert word and set prev word node's nextWord link to this word's node

//  TrieNode * insertWord(const char *word);        					// insert word with no link to prev or next word
//	inline const TrieNode * getNextBranch()      const { return nextBranch; }
//	inline void  setNextBranch(const TrieNode* next)   { nextBranch = next; }
		
	void printWordsAll() const;
	void printNextTree() const;
	void printNextWordMemo();
	void printWordsOfLength(int length) const;
};

#endif // TrieNode_hpp