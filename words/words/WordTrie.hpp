// WordTrie.hpp : word prefix tree with forward links (optimized for speed at the expense of memory)
// Sprax Lines, July 2010

#ifndef WordTrie_hpp
#define WordTrie_hpp

#include "TrieNode.hpp"

class WordTrie 
{
public:
    static const int sMaxWordLen = 28;

    WordTrie() : mRoot(new TrieNode(NULL, 0))  {}
    WordTrie(WordTrie & other);                             // No copy constructor: don't define this.
    WordTrie& operator=(const WordTrie&);                   // Prevent assigment by not defining this.
   ~WordTrie() {}                                           // This class owns no heap memory.

    inline const TrieNode * getRoot()                 const { return mRoot; }
    inline const TrieNode * getFirstWordNode()        const { return mRoot->getFirstWordNode(); }
    inline const TrieNode * getFirstWordNodeFromLetter(int ic) const { return mRoot->getFirstWordNodeFromLetter(ic); }
    inline TrieNode *  insertWord(const char *word, TrieNode *prev=NULL) {
        return mRoot->insertWord(word, prev);
    }
    inline bool             hasWord(const char *key)        const { return mRoot->hasWord(key); }
    inline const TrieNode * subTrie(const char *key)        const { return mRoot->subTrie(key); }
    inline const TrieNode * subTrix(const char *k, int x)   const { return mRoot->subTrix(k, x); }   

    const TrieNode * getNextWordNodeAndIndex(const TrieNode *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const;	// fromBeg==index of 1st letter/node that must change

    inline void printWordsAll()			const { mRoot->printWordsAll(); }


private:
    // const int mWordLength;   // All words must be exactly this long.
    TrieNode *mRoot;            // The root node has no parent, and its depth == 0.
};


#endif // WordTrie_hpp