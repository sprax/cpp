// SubWords.cpp : count number of smaller words in a string
// Sprax Lines,  July 2010

#include "stdafx.h"
#include "SubWords.hpp"

using namespace std;

#define USE_MAP 1

//template <class C>
class SubWords 
{
public:

    WordCPHM    mCharPtrHashMap;
#if 00
    WordCPHS    mCharPtrHashSet;
    WordSSHS    mStringHashSet;
    WordSSSS    mStringSet;
#endif
    IWordContainer<WordT, CountT> *  mWordContainer;
 //   const IWordAdd<T> & mWordContainer;

private:

    int mMaxWordLen;
    string mSpace;

public:

    const static int BUFSIZE     = 32;
    static const int sMinWordLen =  3;

    SubWords() : mMaxWordLen(0), mSpace(" ")
    {}

    CountT isWord(WordT key)
    {
        return mWordContainer->hasWord(key);
    }

    CountT isWord(const string& key)
    {
        return mWordContainer->hasWord(key.c_str());    // zoid: deprecate & remove
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

    CountT numSubWordsWithParse(WordT word, int len, string &parse)
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
            register char * pEnd = const_cast<char *>(word + j);
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
#if 10
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
#endif
    template <typename T>
    CountT maxSubWords(CountT minLen)
    {

        CountT  maxNumWords = 0;
        string  maxParse;

        // zoid IWordAdd<char *>::iterator beg = wordContainer.begin();

#if 0
        for (T::iterator beg = mCharPtrHashMap.begin(), end = mCharPtrHashMap.end(); beg != end; ++beg) {            
            WordT word = beg->first;
            CountT len = beg->second; 
#elif 1
        for (mWordContainer->initIter(); mWordContainer->hasNext(); mWordContainer->goNext()) {
            WordT word = mWordContainer->getWord();
            CountT len = mWordContainer->getSize();
            /*
            static int zoidDbg = 0;
            const string& word = mWordContainer->getWord();
            CountT len = word.length();
            CountT siz = mWordContainer->getSize();
            if (len != siz) {
                zoidDbg++;
            }
            */
#else
    //virtual void  initIter() = 0;
    //virtual bool  hasNext() = 0;
    //virtual void  goNext()  = 0;
    //virtual W     getWord() = 0;
    //virtual S     getSize() = 0;
        for (T::iterator beg = mCharPtrHashMap.begin(), end = mWordContainer.end(); beg != end; ++beg) {
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
                printf("MAX  %2d  %2d  %24s  %s\n", len, maxNumWords, word, maxParse.c_str());
            } else if (numWords > 5) {
                printf("MAX  %2d  %2d  %24s  %s\n", len, maxNumWords, word, parse.c_str());
            }
        }
        return  maxNumWords;
    }

    int initFromDictionaryFile(const char *fname)
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
          //wordContainer.addWord(word, length);
            mWordContainer->addWord(word, length);
            numKept++;
        }
        fclose(fi);
        printf("Read %d words, kept %d, %d <= lengths <= %d\n", numRead, numKept, sMinWordLen, mMaxWordLen); 
        return numKept;
    }

    int test_SubWords(const char *dictFilePath, WordContainerType conType) {
        
        int maxNum = 0, maxLen = 0;
        WordContainerType containerType = conType;
        IWordContainer<WordT, CountT> * iWordContainer = NULL;

            switch (containerType) {
            case eCharPtrHashMap : iWordContainer = &mCharPtrHashMap;
                break;
#if 00
            case eCharPtrHashSet : iWordContainer = &mCharPtrHashSet;
                    break;
            case eStringHashSet  : iWordContainer = &mStringHashSet;
                break;
            case eStringSet      : iWordContainer = &mStringSet;
                break;
#endif
            default:
                printf("Invalid enum type in initFromDictionaryFile\n");
                exit(0);
            }
            
            mWordContainer = iWordContainer;
            const type_info& typeInfo = typeid(mWordContainer);
            const char *typeName = typeInfo.name();
            printf("typeName: %s\n", typeName);

            maxLen = initFromDictionaryFile(dictFilePath);

        if (maxLen < sMinWordLen) {
            printf("Error: longest word length %d < %d\n", maxLen, sMinWordLen);
            return 0;
        }
        TimeT begTime = GetTime();
        maxNum = maxSubWords<CharPtrHashMap>(8);
        TimeT endTime = GetTime();
        printf("test_SubWords: finished in elapsed time %.3f seconds\n", DeltaTimeSeconds(endTime, begTime));
        return maxNum;
    }

};

int test_SubWords(const char *dictFilePath)
{
    SubWords subWords;
    int test = subWords.test_SubWords(dictFilePath, eCharPtrHashMap);
    return test;
}