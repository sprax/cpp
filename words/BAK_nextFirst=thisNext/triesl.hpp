#ifndef triesl_hpp
#define triesl_hpp

#include "TrieSln.hpp"

class TrieSL {
public:
    TrieSL() 
    {
        mRoot = new TrieSln(NULL, 0);
    }
    inline TrieSln *  insertWord(const char *word, TrieSln *prev=NULL)	{ return mRoot->insertWord(word, prev); }
    inline bool       hasWord(const char *key)        const { return mRoot->hasWord(key); }
    inline TrieSln *  subTrie(const char *key)  			{ return mRoot->subTrie(key); }
    inline TrieSln *  subTrix(const char *k, int x)  		{ return mRoot->subTrix(k, x); }
    inline TrieSln *  getRoot()                       const { return mRoot; }

    const TrieSln * getNextWordNodeOld(const TrieSln *keyNode, const char *keyWord, int   fromBeg, int fromEnd) const;	// zoid: use pointer instead of reference for clarity?  fromBeg will be changed!
    const TrieSln * getNextWordNodeRef(const TrieSln *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const;	// zoid: use pointer instead of reference for clarity?  fromBeg will be changed!

    inline void printWordsAll()			const { mRoot->printWordsAll(); }
 
protected:
    TrieSln *mRoot;
//  int		 mWordLength;
};


#endif // triesl_hpp