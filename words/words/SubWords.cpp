// SubWords.cpp : count number of smaller words in a string
// Sprax Lines,  July 2010

#include "stdafx.h"
#include "SubWords.hpp"

using namespace std;

typedef unsigned int CountT;

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

template <typename T>
class IAddWord
{   // Interface methods should almost always be declared public, as is the default in Java.   
public:
    // If access is not public (or unspecified),  instantiating classes may be
    // free to declare their implementation of addWord as private or public, but 
    // objects declared as this interface won't be able to access addWord (which
    // defaults to private).  For example, if a method is defined as:
    // void add(IAddWord<T> & raw, char *word) {raw.addWord(word);} 
    // will fail to compile.
    virtual void addWord(T, CountT=0) = 0;  // access unspecified
};

// BEST, like 0.167 seconds
#include <hash_map>
typedef std::hash_map<char *, unsigned int, std::hash_compare<const char *, lessCharStrCmp>> CharPtrHashMap; 
class WordCPHM : public CharPtrHashMap, public IAddWord<char *>
{
public:
    virtual void addWord(char *tempWord, CountT len)
    {
        char * saveWord = _strdup(tempWord);
        insert(CharPtrHashMap::value_type(saveWord, len));
    }
};

// SECOND, like 0.170 seconds
#include <hash_set>
typedef std::hash_set<char *, std::hash_compare<const char *, lessCharStrCmp>> CharPtrHashSet;
class WordCPHS : public IAddWord<char *>
{
    CharPtrHashSet mWords;
public:
    virtual void addWord(char *tempWord, CountT=0)  // default value for omitted (unnamed) argument
    {
        char * saveWord = _strdup(tempWord);
        mWords.insert(saveWord);
    }
};

// 25% slower
typedef std::hash_set<std::string, std::hash_compare<std::string, lessStringStrCmp>> StringHashSet; 
class WordSSHS : public StringHashSet, IAddWord<char *>
{
public:
    virtual void addWord(char *tempWord, CountT=0)
    {
        std::string saveWord(tempWord);
        insert(saveWord);
    }
};

// set take 3 times as long as hash_set!
#include <set>
typedef std::set<const std::string, lessStringStrCmp> StringSet;  

class WordSSSS : public StringSet, IAddWord<char *>
{
public:
    virtual void addWord(char *tempWord, CountT=0)
    {
        std::string saveWord(tempWord);
        insert(saveWord);
    }
};

class SubWords 
{
private:



    int mMaxWordLen;
    string mSpace;

    typedef enum { eCharPtrHashMap
                 , eCharPtrHashSet
                 , eStringHashSet
                 , eStringSet
    } ContainerType;

public:


    const static int BUFSIZE     = 32;
    static const int sMinWordLen =  3;

    SubWords() : mMaxWordLen(0), mSpace(" ")
    {}


    CountT isWord(char * key)
    {
        return mCharPtrHashMap.count(key);
    }

    CountT isWord(const string& key)
    {
        return mStringHashSet.count(key);
    }



    CountT numSubWords(char * word, int len)
    {
        if (len < 6) {
            return isWord(word);
        }
        CountT maxNumWords = 0;
        int last = len - sMinWordLen;
        for (int j = sMinWordLen; j < last; j++) {
            // Test the right hand substring, that is, the end string:
            CountT rightNumber = numSubWords(word + j, len - j);
            if (rightNumber < 1) {
                continue;
            }
            // Test the left hand substring, that is, the beg string:
            register char * pEnd = word + j;
            register char   cEnd = *pEnd;
            *pEnd = NULL; 
            CountT leftNumber = numSubWords(word, j);
            *pEnd = cEnd;
            if (leftNumber < 1) {
                continue;
            }
            CountT sum = leftNumber + rightNumber;
            if (maxNumWords < sum) {
                maxNumWords = sum;
            }
        }
        return maxNumWords;
    }

    CountT numSubWordsWithParse(char * word, int len, string &parse)
    {
        if (len < 6) {
            if (isWord(word)) {
                parse.append(word).append(mSpace);
                return 1;
            }
            parse.clear();
            return 0;
        }
        CountT maxNumWords = 0;
        int last = len - sMinWordLen;
        for (int j = sMinWordLen; j < last; j++) {
            string leftParse, rightParse;
            // Test the right hand substring, that is, the end string:
            CountT rightNumber = numSubWordsWithParse(word + j, len - j, rightParse);
            if (rightNumber < 1) {
                continue;
            }
            // Test the left hand substring, that is, the beg string:
            register char * pEnd = word + j;
            register char   cEnd = *pEnd;
            *pEnd = NULL; 
            CountT leftNumber = numSubWordsWithParse(word, j, leftParse);
            *pEnd = cEnd;
            if (leftNumber < 1) {
                continue;
            }
            CountT sum = leftNumber + rightNumber;
            if (maxNumWords < sum) {
                maxNumWords = sum;
                parse = leftParse.append(rightParse).append(mSpace);
            }
        }
        return maxNumWords;
    }

    int numSubWordsWithParse(const string& word, int len, string &parse)
    {
        if (len < 6) {
            if (isWord(word)) {
                parse.append(word).append(mSpace);
                return 1;
            }
            parse.clear();
            return 0;
        }
        int maxNumWords = 0;
        int last = len - sMinWordLen;
        for (int j = sMinWordLen; j < last; j++) {
            string left, right;
            string leftString(word.substr(0, j));
            int leftNumber = numSubWordsWithParse(leftString, j, left);
            if (leftNumber < 1) {
                continue;
            }
            string rightString(word.substr(j));
            int rightNumber = numSubWordsWithParse(rightString, len - j, right);
            if (rightNumber < 1) {
                continue;
            }
            int sum = leftNumber + rightNumber;
            if (maxNumWords < sum) {
                maxNumWords = sum;
                parse = left.append(right).append(mSpace);
            }
        }
        return maxNumWords;
    }

    int numSubWords(const string& word, int len)
    {
        if (len < 6) {
            return isWord(word);
        }
        int maxNumWords = 0;
        int last = len - sMinWordLen;
        for (int j = sMinWordLen; j < last; j++) {
            string leftString(word.substr(0, j));
            int leftNumber = numSubWords(leftString, j);
            if (leftNumber < 1) {
                continue;
            }
            string rightString(word.substr(j));
            int rightNumber = numSubWords(rightString, len - j);
            if (rightNumber < 1) {
                continue;
            }
            int sum = leftNumber + rightNumber;
            if (maxNumWords < sum) {
                maxNumWords = sum;
            }
        }
        return maxNumWords;
    }


    CountT maxSubWords(CountT minLen)
    {

        CountT  maxNumWords = 0;
        string  maxParse;

#if USE_CHAR_STAR
#   if USE_MAP
        for (WordHashMap::iterator beg = mWords.begin(), end = mWords.end(); beg != end; ++beg) {            
            char * word = beg->first;
            CountT len  = beg->second; 
#   else
        for (WordSet::iterator beg = mWords.begin(), end = mWords.end(); beg != end; ++beg) {            
            char * word = *beg;
            CountT len  = strlen(word);
#endif
#else
        for (StringHashSet::iterator beg = mStringHashSet.begin(), end = mStringHashSet.end(); beg != end; ++beg) {
            const string& word = *beg;
            CountT len = word.length();
#endif
            if (len < minLen) {
                continue;
            }
            string parse;
            CountT numWords = numSubWordsWithParse(word, len, parse);
            if (maxNumWords < numWords) {
                maxNumWords = numWords;
                maxParse    = parse;

#if USE_CHAR_STAR
                printf("MAX  %2d  %2d  %24s  %s\n", len, maxNumWords, word, maxParse.c_str());
#else
                printf("MAX  %2d  %2d  %24s  %s\n", len, maxNumWords, word.c_str(), maxParse.c_str());
#endif
            } else if (numWords > 5) {
#if USE_CHAR_STAR
                printf("MAX  %2d  %2d  %24s  %s\n", len, maxNumWords, word, parse.c_str());
#else
                printf("MAX  %2d  %2d  %24s  %s\n", len, maxNumWords, word.c_str(), parse.c_str());
#endif
            }
        }
        return  maxNumWords;
    }

    WordCPHM    mCharPtrHashMap;    
    WordCPHS    mCharPtrHashSet;
    WordSSHS    mStringHashSet;
    WordSSSS    mStringSet;
#if 1
    template <typename T>
    int initFromDictionaryFile(const char *fname, ContainerType containerType, IAddWord<T> & wordContainer)
#else
    //template <typename T>
    int initFromDictionaryFile(const char *fname, ContainerType containerType, WordCPHM & wordContainer)
#endif
    {
        FILE *fi;
        if ( ! fopen_s(&fi, fname, "r") ) {
            printf("Opened %s . . . " , fname);
        } else {
            printf("Error opening dictionary file: %s\n", fname);
            return 0;
        }

        char word[BUFSIZE];
        int numRead = 0, numKept = 0;
        while (fgets(word, BUFSIZE, fi))  {
            // got next word as a line in the file; now get its length, and remove any trailing newline.
            WordLenT length = (WordLenT)strlen(word);
            WordLenT lenM1  = length - 1;
            if (word[lenM1] < 'a') {
                word[lenM1] = NULL;
                length = lenM1;
            }
            numRead++;
            if (length < sMinWordLen) {
                continue;
            }
            if (mMaxWordLen < length)  {
                mMaxWordLen = length;
            }

            switch (containerType) {
            case eCharPtrHashMap : wordContainer.addWord(word, length);
                break;
            case eCharPtrHashSet : wordContainer.addWord(word);
                    break;
            case eStringHashSet  : mStringHashSet.addWord(word);
                break;
            case eStringSet      : mStringSet.addWord(word);
                break;
            default:
                printf("Invalid enum type in initFromDictionaryFile\n");
                exit(0);
            }
            numKept++;
        }
        fclose(fi);
        printf("Read %d words, kept %d, %d <= lengths <= %d\n", numRead, numKept, sMinWordLen, mMaxWordLen); 
        return numKept;
    }

    int test_SubWords(const char *dictFilePath) {

        int maxNum = 0;
        int maxLen = initFromDictionaryFile(dictFilePath, eCharPtrHashMap, mCharPtrHashMap);

        if (maxLen < sMinWordLen) {
            printf("Error: longest word length %d < %d\n", maxLen, sMinWordLen);
            return 0;
        }
        TimeT begTime = GetTime();
        maxNum = maxSubWords(8);
        TimeT endTime = GetTime();
        printf("test_SubWords: finished in elapsed time %.3f seconds\n", DeltaTimeSeconds(endTime, begTime));
        return maxNum;
    }

};

int test_SubWords(const char *dictFilePath)
{
    SubWords subWords;
    int test = subWords.test_SubWords(dictFilePath);
    return test;
}
