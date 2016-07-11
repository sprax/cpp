// CharMap.hpp 
// 
// Sprax Lines,  September 2012

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CharFreqMap.hpp"


#ifdef _DEBUG
#if USE_BASE_CHAR_MAP && USE_VIRT_CHAR_IDX	
// If this is 0, then using CharMap as a template argument will lead to slicing:
// default (base class) implementation of pure virtual method
// This is here only for analysis and debugging.
uint CharMap::charToIndex(uchr uc)  const
{
	assert( ! "Base class default impl of pure virt: should never be called!");
	return ((uint) -1) / 2;
}
#endif
#endif


IdentCharMap::IdentCharMap(const CharFreqMap& charFreqMap) 
	: CharMap(charFreqMap.getDomainSpread(), charFreqMap.getMinChar()
    , charFreqMap.getMinChar(), charFreqMap.getMaxChar() )
{ }

CompactCharMap::CompactCharMap(const CharFreqMap& charFreqMap) 
	:  ArrayCharMap(charFreqMap.getRangeCount(), 0, charFreqMap.getMinChar(), charFreqMap.getMaxChar()) 
{
	////mCharToIndexMem = (uint *)  malloc(mDomainSpread*sizeof(uint));
	mCharToIndexMem = new uint[charFreqMap.getDomainSpread()];
	mCharToIndexPtr = mCharToIndexMem - charFreqMap.getMinChar();
	memset(mCharToIndexMem,  (uint)-1, charFreqMap.getDomainSpread()*sizeof(uint));
	charFreqMap.initTableNaturalOrder(mCharToIndexPtr);
}

FreqFirstCharMap::FreqFirstCharMap(const CharFreqMap& charFreqMap, uint minCount) 
	: ArrayCharMap(charFreqMap.getRangeCountOverMin(minCount), 0, charFreqMap.getMinChar(), charFreqMap.getMaxChar())
{
////mCharToIndexMem = (uint *) malloc(sDomainSize*sizeof(uint));
	mCharToIndexMem = new uint[charFreqMap.sDomainSize];
	mCharToIndexPtr = mCharToIndexMem - charFreqMap.sBegChar;
	memset(mCharToIndexMem, charFreqMap.getRangeCount(), charFreqMap.sDomainSize*sizeof(uint));
	charFreqMap.initTableFrequencyOrder(mCharToIndexPtr, targetSize());
}

