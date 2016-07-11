// CharFreqMap.cpp : trie nodes for word tries, 

#include "CharFreqMap.hpp"
#include "wordPlatform.h"

#include <algorithm>
#include <functional>      // For greater<int>()
#include <iostream>

// Constructor
CharFreqMap::CharFreqMap(const char *fileSpec, uint minWordLength, uint maxWordLength, uint minCharCount)
	: mFileSpec(fileSpec), mDefaultCharMap(NULL), mIdentCharMap(NULL), mCompactCharMap(NULL)
	, mFreqFirstCharMap(NULL), mCharCounts(NULL), mMinUchr(sEndChar), mMaxUchr(sBegChar)
    , mMinWordLength(minWordLength), mMaxWordLength(maxWordLength), mMinCharCount(minCharCount)
    , mDomainSpread(0), mRangeCount(0), mMinFoundWordLength(256), mMaxFoundWordLength(0)        // TODO: 256 arbitrary number
    , mNumDistinctChars(0), mNumTotalReadChars(0)
{
    memset(mCharCountsMem, 0, sDomainSize*sizeof(int));
    mCharCounts = mCharCountsMem - sBegChar;
}

CharMap::SubType CharFreqMap::initCountsAndDefaultType(uint minWordLength, uint maxWordLength, int verbosity)
{
    int rangeCount = countCharFreqs(mFileSpec, minWordLength, maxWordLength, verbosity);
    if (rangeCount < 1) {
		printf("WARNING: No words or chars read from file <%s>\n", mFileSpec);
        return CharMap::eUnknownSubType;
	}
	return decideDefaultCharMapType();
 }

uint CharFreqMap::countCharFreqs(const char *fileSpec, uint minWordLength, uint maxWordLength, int verbosity)
{
	mMaxFoundWordLength = countWordCharsInFile(fileSpec, mCharCounts, minWordLength, maxWordLength, mMinFoundWordLength, mMaxFoundWordLength);
    if (mMaxFoundWordLength < minWordLength)
        return -1;
    
	assert(mRangeCount == 0);
	for (uint j = sBegChar; j <= sEndChar; j++) {
		uint count = mCharCounts[j];
		if (count > 0) {
			mNumDistinctChars  += 1;
			mNumTotalReadChars += count;
			if (count >= mMinCharCount) {
				mRangeCount++;
				uchr letter = (uchr) j;
				vcf.push_back(CharFreq(letter, count));
				if (mMinUchr > letter) {			// Must check min before max
					mMinUchr = letter;              // because letter only increases
				} else if (mMaxUchr < letter) {     // inside this loop!
					mMaxUchr = letter;
				}
			}
		}
	}
	sort(vcf.begin(), vcf.end(), CharFreqGreater() );
	printf("Found %d distinct chars, keeping rangeCount=%d of them.\n", mNumDistinctChars, mRangeCount);
	if (verbosity > 1) {
		printf("index\tchar\tcount:\n");
		uint idx = 0;
		for (std::vector<CharFreq>::const_iterator it = vcf.begin(); it != vcf.end() ; ++it, ++idx) {
			printf("%3d\t%c\t%d\n", idx, it->mChar, it->mFreq);
		}
	}

    return mRangeCount;
}


/** Count the chars in the first word in each line of a text file.
*  Count only chars > mMinChar (default minimum uchr == Space),
*  and only in words in the specified range of word lengths.
*  Sets the found minimum and maximum lengths of words found, 
*  even if lesser/greater than the min/maxWordLength arguments.
*/
uint CharFreqMap::countWordCharsInFile(const char *fileSpec, uint charCounts[]
	, uint minWordLength, uint maxWordLength
	, uint& rMinFoundWordLength, uint& rMaxFoundWordLength
	)
{
	FILE *fi;
    uint numWordsUsed = 0;
    uint minFoundWordLength = INT_MAX;
    uint maxFoundWordLength = 0;
	errno_t err = fopen_safe(&fi, fileSpec, "r");
	if (0 == err) {
		printf("Opened file %s\n" , fileSpec);
	} else {
		printf("Error opening file: %s\n", fileSpec);
		return 0;
	}
	char line[sBufSize];
	while (fgets(line, sBufSize, fi))  {
		// Got the next line in the file; now get its length, and remove any trailing newline.
		uint length = extractFirstWord(line);
		assert(length < sBufSize - 2);          // buffer was big enough for word + line ending
		if (minFoundWordLength > length)
			minFoundWordLength = length;
		if (maxFoundWordLength < length)
			maxFoundWordLength = length;

        if (length < minWordLength || maxWordLength < length)
			continue;

        ++numWordsUsed;
		for (uint j = 0; j < length; j++) {
			uchr uch = line[j];
			++charCounts[uch];
		}
	}
	fclose(fi);
    rMinFoundWordLength = minFoundWordLength;
    rMaxFoundWordLength = maxFoundWordLength;
	return numWordsUsed;
}


/** 
*  Extract the word at the start of the input string by finding the first 
*  non-word character and NULL-terminating it there.
*  Return the length of this null-terminated word string. 
*/
uint CharFreqMap::extractFirstWord(char *line)
{
	assert(line != NULL);
	register char *ptrSigned = line;            // pointer to signed char
	for ( ; ; ++ptrSigned) {
		register uchr valUnsigned = *ptrSigned; // value as unsigned char
		if (valUnsigned <= sBegChar) {
			*ptrSigned = NULL;
			break;
		}
	}
	uint   length = ptrSigned - line;   // pointer arithmetic
	return length;
}

CharMap::SubType CharFreqMap::decideDefaultCharMapType()
{
	mDomainSpread = mMaxUchr - mMinUchr + 1;
	int numHoles = mDomainSpread - mRangeCount;
	double foundRatio = (double) numHoles / mRangeCount;
	double asciiRatio = (double) 6.0 / 52.0;
	// If the percentage of holes is less than 1 percent worse than ASCII,
	// then index the trie branches using raw char values.  That is, just
	// shift the chars to map mMinChar to index 0.  Otherwise, transform 
	// the char values into contiguous indices, using some form of look-up 
	// table or some simple function.
	if (foundRatio > asciiRatio + 0.01) {   // TODO: magic number
		mDefaultSubType = CharMap::eFreqFirstCharMap;
	} else if (numHoles > 2) {              // TODO: magic number
		mDefaultSubType = CharMap::eCompactCharMap;
	} else {
		mDefaultSubType = CharMap::eIdentityCharMap;
	}
	return mDefaultSubType;
}

const CharMap	& CharFreqMap::makeDefaultCharMap()	
{
	switch (mDefaultSubType) {
	case CharMap::eFreqFirstCharMap:
		return makeFreqFirstCharMap();
	case CharMap::eCompactCharMap:
		return makeCompactCharMap();
	case CharMap::eIdentityCharMap:
		return makeIdentityCharMap();
	default:
		printf("WARNING: Default subtype of CharMap is not set.  Using IdentCharMap.\n");
		return makeIdentityCharMap();
	}
}

const IdentCharMap& CharFreqMap::makeIdentityCharMap()	
{
	if (mIdentCharMap == NULL)
		mIdentCharMap =  new IdentCharMap(*this);
	return *mIdentCharMap;
}

const CompactCharMap& CharFreqMap::makeCompactCharMap()	
{
	if (mCompactCharMap == NULL) {
		mCompactCharMap =  new CompactCharMap(*this);
	}
	return *mCompactCharMap;
}

const FreqFirstCharMap& CharFreqMap::makeFreqFirstCharMap(uint minCharCount)	
{
	if (mFreqFirstCharMap == NULL) {
		mFreqFirstCharMap =  new FreqFirstCharMap(*this, minCharCount);
	}
	return *mFreqFirstCharMap;
}

uint CharFreqMap::getRangeCountOverMin(uint minCount) const
{
	assert(mRangeCount > 0);	// Counts must already be initialized for const
	if (minCount <= mMinCharCount)
		return mRangeCount;

    int rangeCount = 0;

#if 1	// TODO: use vector's bounds or binary search
	for (std::vector<CharFreq>::const_iterator it = vcf.begin(); it != vcf.end() ; ++it) {
		if (it->mFreq > minCount)
			++rangeCount;
		else
			break;
	}
#else
	for (uint j = mMinUchr; j < mDomainSpread; j++) {
		uint count = mCharCounts[j];
		if (count > minCount) {
			++rangeCount;
		}
	}
#endif
	return rangeCount;
}

/** Use the natural character order (ascending bits), but compacted. */
void CharFreqMap::initTableNaturalOrder(uint *charToIndexPtr)	const
{
	char ch = 0;
	uchr uc = 0;
	uint idx = 0;
	for (uint j = sBegChar; j <= sEndChar; j++) {
		uint count = mCharCounts[j];
		if (count >= mMinCharCount) {
			ch = (  signed char) j;
			uc = (unsigned char) j;
			charToIndexPtr[j] = idx;
			printf("%3d  %3d  %c  %c  %d\n", idx, j, ch, uc, mCharCounts[j]);
			++idx;
		}
	}
}

/** Order the char-to-index table by descending char frequencies. 
*  The most frequent char gets index 0, second-most gets 1, etc.   
**/
void CharFreqMap::initTableFrequencyOrder(uint *freqCharToIndexPtr, uint targetSize)	const
{
	uint idx = 0;
	for (std::vector<CharFreq>::const_iterator it = vcf.begin(); it != vcf.end() ; ++it) {
		uchr uc = it->mChar;
		freqCharToIndexPtr[uc] = idx;
		if (++idx == targetSize)
			break;
	}
}
