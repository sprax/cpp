// SubWords.hpp : 
// Sprax Lines, July 2010

#ifndef SubWords_hpp
#define SubWords_hpp

#include "stdafx.h"

typedef enum { eCharPtrHashMap = 0
    , eCharPtrHashSet = 1
    , eStringHashSet  = 2
    , eStringSet      = 3
} WordContainerType;


int test_SubWords(const char *dictFilePath);


typedef const    char * WordT;
typedef unsigned int    CountT;

template <typename W, typename S>
class IWordAdd
{
public:
    virtual void    addWord(W, S=0)  = 0;
    virtual CountT  hasWord(W)       = 0;
};

template <typename W, typename S>
class IWordIter
{
public:
    virtual void  initIter() = 0;
    virtual bool  hasNext()  = 0;
    virtual void  goNext()   = 0;
    virtual W     getWord()  = 0;
    virtual S     getSize()  = 0;
};

template <typename W, typename S>
class IWordContainer : public IWordAdd<W,S>, public IWordIter<W,S>
{
};


struct lessStringStrCmp
{
  inline bool operator()(const std::string& s1, const std::string& s2) const
  {
      return strcmp(s1.c_str(), s2.c_str()) < 0;
  }
};

struct lessStringLen
{
  inline bool operator()(const std::string& s1, const std::string& s2) const
  {
      return s1.length() < s2.length();
  }
};

struct lessCharStrCmp
{
  inline bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};


// BEST, like 0.167 seconds

typedef std::unordered_map<WordT, CountT, 
	std::hash<WordT>,
	std::equal_to<WordT>
//	std::hash_compare<WordT, lessCharStrCmp>
> CharPtrHashMap;

/** WARNING: This container keeps pointers to char* strings, and only compares those pointers, so it is
only good for tracking the original input strings, not for comparing other strings that may have the same
content.
*/
class WordCPHM : public CharPtrHashMap, public IWordContainer<WordT, CountT>
{
public:
    virtual void addWord(WordT tempWord, CountT len)
    {
        char * saveWord = _strdup(tempWord);
        insert(CharPtrHashMap::value_type(saveWord, len));
    }
    virtual CountT hasWord(WordT word) {
        return count(word);
    }
    virtual void  initIter() {
        mBeg = begin();
        mEnd = end();
    }
    virtual bool    hasNext() {
        return mBeg != mEnd;
    }
    virtual void    goNext() {
        ++mBeg;
    }
    virtual WordT   getWord() {
        return mBeg->first;
    }
    virtual CountT  getSize() {
        return mBeg->second;
    }
protected:
    CharPtrHashMap::iterator mBeg, mEnd;
};

// SECOND, like 0.170 seconds
typedef std::unordered_set<char *, std::hash_compare<const char *, lessCharStrCmp>> CharPtrHashSet; 
class WordCPHS : public IWordContainer<WordT, CountT>
{
    CharPtrHashSet mWords;
public:
    virtual void addWord(WordT tempWord, CountT=0)  // default value for omitted (unnamed) argument
    {
        char * saveWord = _strdup(tempWord);
        mWords.insert(saveWord);
    }
};

// 25% slower
typedef std::unordered_set<std::string, std::hash_compare<std::string, lessStringStrCmp>> StringHashSet; 
class WordSSHS : public StringHashSet, public IWordContainer<WordT, CountT>
{
public:
    virtual void addWord(WordT tempWord, CountT=0)
    {
        std::string saveWord(tempWord);
        insert(saveWord);
    }
};

// set take 3 times as long as hash_set!
typedef std::set<std::string, lessStringStrCmp> StringSet;  

class WordSSSS : public StringSet, public IWordContainer<WordT, CountT>
{
public:
    virtual void addWord(WordT tempWord, CountT=0)
    {
        std::string saveWord(tempWord);
        insert(saveWord);
    }
};


#endif // SubWords_hpp