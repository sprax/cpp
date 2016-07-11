//  WordNode.hpp : trie nodes for word tries
//  Sprax Lines,  July 2010
//
//  TODO: EvOdNode::evalIsWinner and isWinnerMemo() should only be called after
//  the Trie that  contains it is fully loaded, wheres updateNodeStatus can be
//  called during loading.  Either simplify and optimize to the point where they
//  are interchangeable, or create a flag that indicates whether the Trie is 
//  completely loaded yet.

#ifndef EvOdNode_hpp
#define EvOdNode_hpp

//#include "stdafx.h" // zoid
#include "TrieNode.hpp"

// TODO: Move debugging counters into a util src file
#ifdef _DEBUG    
static       int fsCounter =  0;    // file-scope counter for debugging, etc.
#define SET_COUNT(_n_)  (fsCounter  = (_n_))
#define ADD_COUNT(_n_)  (fsCounter += (_n_))
#else
static const int fsCounter = -1;
#define SET_COUNT(_n_)  ((void)0)
#define ADD_COUNT(_n_)  ((void)0)
#endif


/** EvodWordNode : TrieNode specialized for word games depending on even/odd word lengths.
 */

class EvOdNode : public TrieNode
{
private:
    EvOdNode  * mParent;        // One level up in the trie tree is the node representing this stem's "prefix" (the string w/o last this one's last letter)
    const char* mEndWord;       // A minimal-length word that will end the game, if played optimally (the alphabetically first one, if thare > 1) 
    WordLenT    mEndDepth;      // Length of sucn an end word
    WordLenT    mIsWinnerMemo;  // -1 => uninitialized; 1 => true, this node is a winner; 0 => false, this node is a loser.  Once inited, it never changes.
    bool        mIsWinner;      // TODO: Don't really need both mIsWinnerMemo and mIsWinner

// protected:                   // Make these protected, not private, if there are to be any subclasses.
    EvOdNode (const EvOdNode&);                     // Do not define
    EvOdNode& operator=(const EvOdNode&);			// Do not define.
   ~EvOdNode() {}								    // This class owns nothing on the heap.


public:
    EvOdNode(EvOdNode *parent, WordLenT depth, const char *&stem) : TrieNode(depth, stem), mParent(parent)
        , mEndWord(NULL), mEndDepth(0), mIsWinnerMemo((WordLenT)-1), mIsWinner(false)
    {}

    // TODO: sort out getIsWinner-accessor vs evalIsWinner-const vs. isWinnerMemo-memoize vs. updateNodeStatus(word, wordLen):
    //       maybe:  public getIsWinner, protected evalIsWinner, public isWinnerMemoize, & protected updateStatus
    //       Note that isWinnerMemoize would only be useful if all the words are already loaded, and they were 
    //       loaded without updating their status.  As of now, we always pay the cost of evaluating upfront, while loading.  
    //       Loading quickly and memoizing later might be useful if the dictionary needes to be re-loaded often,
    //       and is not necessarily traversed heavily before having to re-loaded again.

    //      GETS & SETS:
    inline          bool        getIsWinner()     const { return mIsWinner; }   // Accessor gets status without re-evaluating anything.  See *IsWinner below.
    inline virtual  EvOdNode *  getParent()	     const { return mParent; }		// NULL, if root node
    inline const    EvOdNode *  getFirstChild() const { return static_cast<const EvOdNode *>(mFirstChild); }	
    inline const    EvOdNode *  getNextSibling()	 const { return static_cast<const EvOdNode *>(mNextBranch); }
    inline virtual  EvOdNode *  getBranchAtIndex(int ix) const {
        return static_cast<EvOdNode*>(TrieNode::getBranchAtIndex(ix));
    }
    inline virtual const EvOdNode * getBranchAtLetter(int letter) const {
        return static_cast<const EvOdNode *>(TrieNode::getBranchAtLetter(letter));
    }
    inline          WordLenT    getEndDepth()    const { return mEndDepth; }
    inline const    char     *  getEndWord()     const { return mEndWord; }
    inline          void        setEndWord(const char * word) { mEndWord = word; }

    void                        printChildWordsAndEndDepths() const;

    static          int         unitTest();


    virtual         EvOdNode * addWord(const char *word, WordLenT wordLen, EvOdNode *) {
        return addWordAndNumbersForward(word, wordLen);   
    //  return addWordAndNumbersBackUp(word, wordLen, prevWordNode);   
    //  return addWordAndWinLose(word, wordLen, prevWordNode);   
    }

    inline           bool       isWinnerMemo()    const {   // Pseudo-const Memoizer computes node's status and/or remembers it.
        if (mIsWinnerMemo == -1) {                          // This node's win/lose status has not yet been evaluated.
            EvOdNode *node = const_cast<EvOdNode *>(this);  // Memoization means this method is only "conceptually const"
            node->mIsWinnerMemo = evalIsWinner();           // auto-conversion of bool to integer type is OK
        }
        return (mIsWinnerMemo == 1);            
    }

    // zoid: following should be protected, but in inner class ??
    bool    evalIsWinnerGetEnds(WordLenT & maxDepth, const char **pMaxLenWord)   const;     // Evaluator for unit tests only: re-computes info even if already stored

protected:

    bool    evalIsWinner()    const;                                                        // Evaluator computes node's status without changing it.  Called by isWinnerMemo.


    // updateNodeStatus: Update this node's winner/loser status and its end word and end depth.
    // Return true if this node is changed, false if not.
    // This method is to be used only while loading the (as of yet, incomplete) Trie.
    // TODO: It could be implemented more efficiently; e.g., it could be made faster 
    // if each node stores the number of winner children it has already.
    bool    updateNodeStatus();

    EvOdNode * addWordAndNumbersForward(const char *word, WordLenT wordLen);
    EvOdNode * addWordAndNumbersBackUp(const char *word, WordLenT wordLen, EvOdNode *prevWordNode);
    EvOdNode * addWordAndWinLose(const char *word, WordLenT wordLen, EvOdNode *prevWordNode);    
    EvOdNode * addWordOld(const char *word, WordLenT wordLen, EvOdNode *prevWordNode);
};

#endif // EvOdNode_hpp