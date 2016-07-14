#ifndef TrieTree_hpp
#define TrieTree_hpp

#include "TrieNode.hpp"

class TrieTree 
{
public:
    static const int sMaxWordLen = 32;

    TrieTree() : mRoot(new TrieNode(NULL, 0))  {}          // root has no parent or predecessor, and depth = 0
    ~TrieTree() {}                                         // this class owns no memory
    TrieTree& operator=(const TrieTree&);                  // prevent assigment by not defining this.

    inline const TrieNode *  getRoot()                    const { return mRoot; }
    inline const TrieNode *  getFirstWordNode()           const { return mRoot->getFirstWordNode(); }
    inline const TrieNode *  getFirstWordNodeFromLetter(int ic) const   { return mRoot->getFirstWordNodeFromLetter(ic); }
    inline TrieNode * insertWord(const char *word, TrieNode *prev=NULL)	{ return mRoot->insertWord(word, prev); }
    inline bool              hasWord(const char *key)     const { return mRoot->hasWord(key); }
    inline const TrieNode *  getWordNode(const char *key) const { return mRoot->getWordNode(key); }
    inline const TrieNode *  getStemNode(const char *key) const { return mRoot->getStemNode(key); }
    inline TrieNode *  subTrie(const char *key)  		{ return mRoot->subTrie(key); }
    inline TrieNode *  subTrix(const char *k, int x)  	{ return mRoot->subTrix(k, x); }   

    const TrieNode * getNextWordNodeAndIndex(const TrieNode *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const;	// fromBeg==index of 1st letter/node that must change

    inline void printWordsAll()			const { mRoot->printWordsAll(); }

    int unit_test() { 
        test_trie_insert_print();  
        return 0;
    }

protected:
    static void initFromFile(TrieTree tries[], WordMap maps[], const char *fname);
    static void test_trie_insert_print();

    TrieNode *mRoot;
    //  int		 mWordLength;
};


#endif // TrieTree_hpp