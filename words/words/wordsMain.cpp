// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "arrayUtil.h"
#include "strUtil.h"

#include "ArrayAlgo.hpp"
#include "BinLink.hpp"
#include "BinTree.hpp"
#include "BoggleBoard.hpp"
#include "Sum123.hpp"
#include "RandModN.hpp"
#include "slicingProblem.h"
#include "maxSubArrays.h"
#include "printArray.h"
#include "tests.h"

#include "FieldInt.hpp"

static int sDbg = 1;

/*
#ifndef WIN32
#include <stdio.h>
#include <map>
#ifdef _DEBUG
#include <assert.h>
#else
#define NDEBUG          1
#define assert(_expr_)  ((void)0)
#endif
#endif
*/

class letterArray
{
public:
    static const int sNumLetters = 26;
    int              mNumNonZero;   // number of distinct letters counted
    int             *letterCounts;  // pointer to allow addressing by char ('a' thru 'z')
    letterArray() : mNumNonZero(0)
    {
        for (int j = sNumLetters; --j >= 0; )
            mMem[j] = 0;
        letterCounts = mMem - 'a';  // address letters by 'a' through 'z'
    }
    void addLetter(char  letter) {
        assert('a' <= letter && letter <= 'z');
        if (++letterCounts[letter] == 1)
            ++mNumNonZero; 
    }
private:
    int mMem[sNumLetters];          // memory to hold the letter counts
};


/*
TODO:

WordTrie in its own project.    template?
*/

/**
reassembleCutupRotatedSortedString(string[] ars)
reassembleCutupRotatedSortedCompleteString(string[] ars)

ideas:
1) cut up further into pairs from the get to, or, probably better, only as needed
2) use pair of hash tables or sorted hash maps, mapping strings to first and last letter, and sorting keys on
   number of distinct letters (or using an array of tables, indexed by number of unique chars.
3) Represent as DAG(s).  First char in the set is the one that has only edges going out, not edges coming in.  
   Next is the one whose incoming edges come only from the first, etc. (topological sort).
4) the "dumb" way: no DS except strings, just clever splicing.
*/
class ReassembleCutupRotatedSortedCompleteString
{
public:
    /** return number of distinct lower case letters in str not already marked in foundLetters */
    static int countNewCharsInRange(const char *str, char first, char last, bool foundChars[]) 
    {
        // no check for str == NULL
        int numNew = 0;
        for (char cc; cc = *str++ - first; ) {
            if (cc < first || cc > last)
                return -1;
            if (foundChars[cc] == false) {
                foundChars[cc] =  true;
                numNew++;
            }
        }
        return numNew;
    }

    /** return number of distinct letters in [a ... z], or -1 if any other char is present.
    */
    // this is a sufficient but not necessary condition for the graph being connected.
    static int countOnlyDistinctLowerCaseLetters(char* ars[], int strCount)
    {
        int  letterCount = 0, temp;
        bool foundLetters[26] = { false, };
        for (int j = 0; j < strCount; j++) {
            temp = countNewCharsInRange(ars[j], 'a', 'z', foundLetters); 
            if (temp < 0)
                return temp;    // character out of range
            letterCount += temp;
            if (letterCount == 26)
                break;
        }
        return letterCount;
    }

    static char * decodeAndReassemble(char *ars[128], int strCount)
    {
        if (ars == NULL || strCount < 1)
            return NULL;
        if (strCount == 1)
            return ars[0];

        // return val
        char *reassembledString = NULL;
        
        int *strLengths = (int *)calloc(strCount, sizeof(int));

        // 1st pass: try these heuristics:
        // 1) Only the letter rotated from 'a' can ONLY be at the start
        // of any substring in which it appears.  It could be repeated
        // there, so it's max position is not 0, but it's max first position
        // will be 0 -- that is, if it is present at all.
        // Other letters could be first in some of the strings, but the 
        // 'a'-letter will have 0 for its maximum FIRST position.  If no other
        // letters can claim this, then we've found the 'a'-letter.
        // 2) For each letter, count the occurrances of letters that precede it
        //    in any substring.
        //    One and only one letter will have no precedents, and it must
        //    be the minimal letter present in the original string.  If all
        //    26 letters are present, this must be the 'a'-letter, and then we
        //    can compute the rotator.  If only 25 letters are present, this
        //    encoded minimal letter could be either 'a' or 'b', and so on.
        //    But even if we cannot tell whether it came from 'a' or 'b'. we
        //    can still put back together at least the beginning of the 
        //    original string.  TODO: Why not necessarily the whole thing?
        //
        // 3) Cutting up a single medium length sorted, rotated string is 
        //    likely to produce a forst of disconnected trees.  A sequence
        //    of sorted, rotated words is more likely to yield a solution.
        //    In that case, the RTTD is to stop processing words as soon
        //    as the a-character is found.  So worst case only happens if
        //    not all 26 letters are present.  (Note that lower-casing all
        //    words as they come in is not cheating, but may throw away
        //    some information.
        int maxFirstPos[26] = { -1, };     // initialize to invalid index
        letterArray lettersBefore[26];
        int k, q, totalLen = 0; 
        char *pc, cc, cprev;
        for (int j = 0; j < strCount; j++) {
            for (k = 0, cprev = 0, pc = ars[j]; (cc = *pc++); cprev = cc, k++) {
                if (cc != cprev) {
                    q = cc - 'a';
                    if (maxFirstPos[q] < k)
                        maxFirstPos[q] = k;
                    if (cprev != 0)
                        lettersBefore[q].addLetter(cprev);
                }
            }
            strLengths[j] = k;
            totalLen += k;
        }
        int numDistinctLetters = 26;
        int numLettersWithMaxPos0 = 0, lastIndexOfMaxPos0 = -1;
        for (k = 0; k < 26; k++) {
            if (maxFirstPos[k] == -1)
                numDistinctLetters--;
            else if (maxFirstPos[k] == 0) {
                numLettersWithMaxPos0++;
                lastIndexOfMaxPos0 = k;
            }
        }
        if (numLettersWithMaxPos0 == 1) {
            printf("\n    Found unique char '%c' with max 1st pos 0, so 'a' was rotated by %d\n"
                , 'a' + lastIndexOfMaxPos0, lastIndexOfMaxPos0);
            reassembledString = reassemble(ars, strCount, strLengths, totalLen, 26 - lastIndexOfMaxPos0);
        }

        int numLettersWithNoPrecedents = 0, lastIndexOfZeroPrecedents = -1;
        for (int j = 0; j < 26; j++) {
            if (lettersBefore[j].mNumNonZero == 0) {
                numLettersWithNoPrecedents++;
                lastIndexOfZeroPrecedents = j;
                if (sDbg > 2)
                    printf("The letter %c (%2d) has no precedents\n", (char)(j + 'a'), j);
            }
        }
        if (numLettersWithNoPrecedents == 1) {
            printf("\n    Found unique char '%c' with no precedents and all 26 present, so 'a' was rotated by %d\n"
                , 'a' + lastIndexOfZeroPrecedents, lastIndexOfZeroPrecedents);
        } else if (sDbg > 0) {
            printf(" %2d", numLettersWithNoPrecedents );
        }
        //// Is the set complete over the known alphabet?
        //// Determine if all and only the 26 lowercase letters are present.
        //int letterCount = countOnlyDistinctLowerCaseLetters(ars, strCount);
        //if (letterCount == 26) {    // got all the letters
        //}
        free(strLengths);
        return reassembledString;
    }

    static char *reassemble(char *ars[], int strCount, int  strLengths[], int totalLen, int rotator)
    {
        for (int j = 0; j < strCount; j++) {
            rotateStringMod26(ars[j], rotator);
        }
        qsort_s(ars, strCount, sizeof(char *), qsort_s_strcmp, NULL);
        char *reassembledStr = (char *)malloc(1 + totalLen * sizeof(char));
        reassembledStr[0] = '\0';
        for (int k = 0; k < strCount; k++) {
            strncat_s(reassembledStr, totalLen, ars[k], strLengths[k]);
        }
        return reassembledStr;
    }

    static int unit_test()
    {
        char *strA = "abc";
        char *strB = "xyz";
        char letters[16] = "abcxyz";
        char *test = letters;
        printf("test rot %2d: %s\n",  0, test);
        test = rotateStringMod26(test,  13);
        printf("test rot %2d: %s\n", 13, test);
        test = rotateStringMod26(test,   7);
        printf("test rot %2d: %s\n",  7, test);
        test = rotateStringMod26(test,  -4);
        printf("test rot %2d: %s\n", -4, test);
        test = rotateStringMod26(test,  10);
        printf("test rot %2d: %s\n", 10, test);
        printf("strcmp(%s, %s) == %d\n", strA, strB, strcmp(strA, strB));
        printf("strcmp(%s, %s) == %d\n", strA, strA, strcmp(strA, strA));
        printf("strcmp(%s, %s) == %d\n", strB, strA, strcmp(strB, strA));
        printf("qsort_strcmp(%s, %s) == %d\n", strA, strB, qsort_strcmp(&strA, &strB));
        printf("qsort_strcmp(%s, %s) == %d\n", strA, strA, qsort_strcmp(&strA, &strA));
        printf("qsort_strcmp(%s, %s) == %d\n", strB, strA, qsort_strcmp(&strB, &strA));
        printf("qsort_s_strcmp(%s, %s) == %d\n", strA, strB, qsort_s_strcmp(NULL, &strA, &strB));
        printf("qsort_s_strcmp(%s, %s) == %d\n", strA, strA, qsort_s_strcmp(NULL, &strA, &strA));
        printf("qsort_s_strcmp(%s, %s) == %d\n", strB, strA, qsort_s_strcmp(NULL, &strB, &strA));

        const char *original = "aaaaaaaaabbbcccddddeeeeeeeeeffffffgggggggggghhhhiiiiiiiijjjjjjjjjjkkklllllllllllmmmmmnnooooopppqqqqrrrrrssttuuuvvvvwwwwwxxxxxyyyyyyyzzz";
        const char *restored = NULL;
        int len = strlen(original);
        //const char **substrs = (const char **)calloc(len, sizeof(const char *)); // TODO
        char *substrs[128];

        for (int q = 0; q < 1000; q++) {
            time_t iterTime = time(NULL);
            unsigned int seed = (unsigned int)((iterTime - q*99) % 1257);
            srand(seed);
            int   strCount = 0;
            substrs[strCount++] = substring(original, 0, 4);
            for (int r, j = 4; j < len; j += r, strCount++) {
                r = 3 + rand() % 6;                             // TODO: make this 1 + rand ... ?
                substrs[strCount] = substring(original, j, r);
            }
            // "encode" the substrings using a constant rotation (add const mod 26)
            // re-seed the RNG or else you'll get a strong correlation between the 
            // rotator and the cut-up partition, which means you'll get repeating results.
            seed = (unsigned int)((iterTime - q*351) % 2453);
            srand(seed);
            int rotator = rand() % 26;
            for (int j = 0; j < strCount; j++) {
                rotateStringMod26(substrs[j], rotator);
            }
            if (sDbg > 1)
                printf("Rotator is %c (%2d)\n", (char)(rotator + 'a'), rotator);
            randomShuffle(substrs, strCount, seed);
            /*
            qsort(substrs, strCount, sizeof(char *), compare);
            shuffleArray(substrs, strCount, seed);
            qsort(substrs, strCount, sizeof(const char *), qsort_strcmp);
            shuffleArray(substrs, strCount, seed);
            qsort_s(substrs, strCount, sizeof(char *), qsort_s_strcmp, NULL);
            shuffleArray(substrs, strCount, seed);
            qsort_s(substrs, strCount, sizeof(char *), qsort_s_strcmp, NULL);
            shuffleArray(substrs, strCount, seed);
            */
            restored = decodeAndReassemble(substrs, strCount);
        }
    
        if (restored != NULL && strncmp(original, restored, len) == 0) {
            printf("\n  reassembled: %s\n", restored);
            return 0;
        }
        printf("\n  reassembly failed\n");
        return -1;
    }
};

//////////////////////////////////////////////////////////////////
int test_AVL_Tree_insert();

int main(int argc, char* argv[])
{
    //make_heap(1, 3);

    int status = 0;
    bool testAll = true;
    if  (testAll) {
        status += test_rotatedString();
        //status += test_isArrayConsecutive(isArrayConsecutiveMod_bust, 23, -4);
        //status += BinLink::unit_test();
        status += BoggleBoard::unit_test();
        status += RandModN::unit_test();
        status += ReassembleCutupRotatedSortedCompleteString::unit_test();
        status += Sum123::unit_test();

        status += BinLink::unit_test();

        status += test_bitvector();
        status += test_boolFunctionOneInt_MinMax( divisible_6_9_17,  -1, 55);
        status += test_boolFunctionOneInt_MinMax( divisible_6_11_19, -1, 55);
        status += test_charString();
        status += test_circleStart(5, 1);
        status += test_divisible_6_9_17(-1, 55);
        status += test_fibonacci();
        status += test_fibonacci_viscomp();
        status += test_for_each();
        status += test_interleaveInPlace();
        status += test_isArrayConsecutive(isArrayConsecutiveConst,    23, -4); 
        status += test_isArrayConsecutive(isArrayConsecutiveModSwap,  23, -4); 
        status += test_Kadane();
        status += test_macro();
        status += test_multimap();
        status += test_NlightBulbs(100);
        status += test_slicingProblem();
        status += test_sqrt_bisection();
        status += test_threeWayPartition();
        status += test_uniqueCharSubString();
        status += ArrayAlgo::unit_test(1);
        //status += test_3dtransformations(argc, argv);
		//testNamedCtorHiddenDtor();
        status += FieldInt::unit_test();

    }

    // status += test_bs(3);

    // test_heaper();

    test_AVL_Tree_insert();

    //getchar(); // modify commnad window properties while waiting for input
    return status;
}

