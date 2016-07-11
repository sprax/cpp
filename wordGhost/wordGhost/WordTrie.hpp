// WordTrie.hpp : word prefix tree with forward links (optimized for speed at the expense of memory)
// Sprax Lines, July 2010

#ifndef WordTrie_hpp
#define WordTrie_hpp

#include "WordNode.hpp"

class WordTrie 
{
public:
    WordTrie(int wordLen) : mRoot(new WordNode(NULL, 0, TrieNode::sEmptyStr)), mWordLen(wordLen), mAlreadyLoaded(false) {}
    WordTrie(WordTrie & other);                             // No copy constructor: don't define this.
    WordTrie& operator=(const WordTrie&);                   // Prevent assigment by not defining this.
   ~WordTrie() {}                                           // This class owns no heap memory.

    inline const WordNode * getRoot()                 const { return mRoot; }
    inline       int        getWordLen()              const { return mWordLen; }
    inline const WordNode * getFirstWordNode()        const { return mRoot->getFirstWordNode(); }
    
    inline const WordNode * getFirstWordNodeFromLetter(int ic)  const { return mRoot->getFirstWordNodeFromLetter(ic); }

    inline WordNode *  addWord(const char *word, WordLenT wordLen, WordNode *prevWordNode)	{ 
        assert(wordLen == mWordLen);
        return mRoot->addWord(word, wordLen, prevWordNode);
    }
    inline bool       hasWord(const char *key)              const { return mRoot->hasWord(key); }
    inline const WordNode * subTrie(const char *key)        const { return mRoot->subTrie(key); }
    inline const TrieNode * subTrix(const char *k, int x)   const { return mRoot->subTrix(k, x); }  // return type ramains TrieNode 

    const WordNode *getNextWordNodeAndIndex(const WordNode *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const;	// fromBeg==index of 1st letter/node that must change

    inline void     printWordsAll()		const   { mRoot->printWordsAll(); }
    bool            getAlreadyLoaded()  const   { return mAlreadyLoaded; }
    void            setAlreadyLoaded(bool done) { mAlreadyLoaded = done; }

private:
    const int mWordLen;           // All words must be this long.
    WordNode *mRoot;              // The root node has no parent, and its depth == 0.
    bool      mAlreadyLoaded;     // The trie is already initialized (as from inserting every word from a dictionary).
};


#endif // WordTrie_hpp