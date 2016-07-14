// charString.cpp : functions on C-style strings: NULL-terminated char arrays
//

#include "stdafx.h"


/**
The Boyer Moore algorithm is the fastest string searching algorithm. Most editors use this algorithm.
It compares the pattern with the actual string from right to left. Most other algorithms compare
from left to right. If the character that is compared with the rightmost pattern symbol does not
occur in the pattern at all, then the pattern can be shifted by m positions behind this text symbol.

The following example illustrates this situation.

Example:

0 1 2 3 4 5 6 7 8 9 ...
a b b a d a b a c b a
| |
b a b a c |
<------ |
|
b a b a c


The comparison of "d" with "c" at position 4 does not match. "d" does not occur in the pattern.
Therefore, the pattern cannot match at any of the positions 0,1,2,3,4, since all corresponding
windows contain a "d". The pattern can be shifted to position 5.
The best case for the Boyer-Moore algorithm happens if, at each search attempt the first compared
character does not occur in the pattern. Then the algorithm requires only O(n/m) comparisons.

Bad character heuristics

This method is called bad character heuristics. It can also be applied if the bad character
(the character that causes a mismatch), occurs somewhere else in the pattern.
Then the pattern can be shifted so that it is aligned to this text symbol.
The next example illustrates this situation.

Example:

0 1 2 3 4 5 6 7 8 9 ...
a b b a b a b a c b a
|
b a b a c
<----
|
b a b a c

Comparison between "b" and "c" causes a mismatch. The character "b" occurs in the pattern at positions 0 and 2. The pattern can be shifted so that the rightmost "b" in the pattern is aligned to "b".


Good suffix heuristics

Sometimes the bad character heuristics fails. In the following situation the comparison between "a" and "b" causes a mismatch. An alignment of the rightmost occurence of the pattern symbol a with the text symbol a would produce a negative shift. Instead, a shift by 1 would be possible. However, in this case it is better to derive the maximum possible shift distance from the structure of the pattern. This method is called good suffix heuristics.

Example:


0 1 2 3 4 5 6 7 8 9 ...
a b a a b a b a c b a
| | |
c a b a b
<----
| | |
c a b a b


The suffix "ab" has matched. The pattern can be shifted until the next occurence of ab in the pattern is aligned to the text symbols ab, i.e. to position 2.


In the following situation the suffix "ab" has matched. There is no other occurence of "ab" in the pattern.Therefore, the pattern can be shifted behind "ab", i.e. to position 5.

Example:


0 1 2 3 4 5 6 7 8 9 ...
a b c a b a b a c b a
| | |
c b a a b
c b a a b



In the following situation the suffix "bab" has matched. There is no other occurence of "bab" in the pattern. But in this case the pattern cannot be shifted to position 5 as before, but only to position 3, since a prefix of the pattern "ab" matches the end of "bab". We refer to this situation as case 2 of the good suffix heuristics.

Example:


0 1 2 3 4 5 6 7 8 9 ...
a a b a b a b a c b a
| | | |
a b b a b
a b b a b



The pattern is shifted by the longest of the two distances that are given by
the bad character and the good suffix heuristics.

The Boyer-Moore algorithm uses two different heuristics for determining the
maximum possible shift distance in case of a mismatch: the "bad character"
and the "good suffix" heuristics. Both heuristics can lead to a shift
distance of m. For the bad character heuristics this is the case, if the
first comparison causes a mismatch and the corresponding text symbol does
not occur in the pattern at all. For the good suffix heuristics this is
the case, if only the first comparison was a match, but that symbol does
not occur elsewhere in the pattern.
Posted by Vijay Agrawal at 2:17 PM
4 comments:

in 1st method some mistakes r there i think.. there it should be x(i+j)==y(i) and if(i>=n).. i think.
verify that one
November 19, 2008 2:32 AM
Udit said...
**/

// Boyer–Moore string search algorithm

#include "limits.h"
#include "string.h"

static const size_t ALPHABET_SIZE = (1 << CHAR_BIT);

static void compute_prefix(const char* str, size_t size, size_t result[ALPHABET_SIZE])
{
    size_t q, k;
    result[0] = 0;

    k = 0;
    for (q = 1; q < size; q++) {
        while (k > 0 && str[k] != str[q])
            k = result[k - 1];

        if (str[k] == str[q])
            k++;
        result[q] = k;
    }
}

static void prepare_badcharacter_heuristic(const char *str, size_t size,
    size_t result[ALPHABET_SIZE]) {

    size_t i;

    for (i = 0; i < ALPHABET_SIZE; i++)
        result[i] = size;

    for (i = 0; i < size; i++)
        result[(size_t)str[i]] = i;
}

#if 00
static void initializeRightmost(char *pattern, size_t patternLen)
{
    mRightmostArr = new int[ALPHABET_SIZE];
    for (int c = 0; c < mCharSetSize; c++)
        mRightmostArr[c] = -1;
    for (int j = 0; j < mPatArr.length; j++)
        mRightmostArr[mPatArr[j]] = j;
}

void prepare_goodsuffix_heuristic(const char *normal, size_t size,
    size_t result[size + 1]) {

    char *left = (char *)normal;
    char *right = left + size;
    char reversed[size + 1];
    char *tmp = reversed + size;
    size_t i;

    /* reverse string */
    *tmp = 0;
    while (left < right)
        *(--tmp) = *(left++);

    size_t prefix_normal[size];
    size_t prefix_reversed[size];

    compute_prefix(normal, size, prefix_normal);
    compute_prefix(reversed, size, prefix_reversed);

    for (i = 0; i <= size; i++) {
        result[i] = size - prefix_normal[size - 1];
    }

    for (i = 0; i < size; i++) {
        const size_t j = size - prefix_reversed[i];
        const size_t k = i - prefix_reversed[i] + 1;

        if (result[j] > k)
            result[j] = k;
    }
}

/*
* Boyer-Moore search algorithm   aabbaabbaabb
*/
const char *boyermoore_search(const char *haystack, const char *needle) {
    /*
    * Calc string sizes
    */
    size_t needle_len, haystack_len;
    needle_len = strlen(needle);
    haystack_len = strlen(haystack);

    /*
    * Simple checks
    */
    if (haystack_len == 0)
        return NULL;
    if (needle_len == 0)
        return haystack;

    /*
    * Initialize heuristics
    */
    size_t badcharacter[ALPHABET_SIZE];
    size_t goodsuffix[needle_len + 1];

    prepare_badcharacter_heuristic(needle, needle_len, badcharacter);
    prepare_goodsuffix_heuristic(needle, needle_len, goodsuffix);

    /*
    * Boyer-Moore search
    */
    size_t s = 0;
    while (s <= (haystack_len - needle_len))
    {
        size_t j = needle_len;
        while (j > 0 && needle[j - 1] == haystack[s + j - 1])
            j--;

        if (j > 0)
        {
            size_t k = badcharacter[(size_t)haystack[s + j - 1]];
            size_t m;
            if (k < j && (m = j - k - 1) > goodsuffix[j])
                s += m;
            else
                s += goodsuffix[j];
        }
        else
        {
            return haystack + s;
        }
    }

    return NULL; // not found
}
#endif // 00

