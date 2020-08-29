// charString.cpp : functions on C-style strings: NULL-terminated char arrays
//

#include "stdafx.h"
#include "linkedLists.h"

/* BSD brute force
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 * History:
 *  2002-01-24 	gvdl	Initial implementation of strstr
 */

//#include <string.h>
//#include <stdlib.h>

 /*
  * Find the first occurrence of find in s.
  */
 char * strstr_FreeBSD(const char *s, const char *find)
 {
     char c, sc;
     size_t len;

     if ((c = *find++) != 0) {
         len = strlen(find);
         do {
             do {
                 if ((sc = *s++) == 0)
                     return (NULL);
             } while (sc != c);
         } while (strncmp(s, find, len) != 0);
         s--;
     }
     return const_cast<char *>(s);  // cast away constness
 }


 char *strstr_old_BSD(const char *in, const char *str)
 {
     char c;
     size_t len;

     c = *str++;
     if (!c)
         return (char *) in;	// Trivial empty string case

     len = strlen(str);
     do {
         char sc;

         do {
             sc = *in++;
             if (!sc)
                 return (char *) 0;
         } while (sc != c);
     } while (strncmp(in, str, len) != 0);

     return (char *) (in - 1);
 }

#ifdef FIXME  // strstr

Write C code to implement the strstr() (search for a substring) function.
This is also one of the most frequently asked interview questions. Its asked almost 99% of the times. Here are a few C programs to implement your own strstr() function.

There are a number of ways to find a string inside another string. Its important to be aware of these algorithms than to memorize them. Some of the fastest algorithms are quite tough to understand!.


Method1
Method2

The second method is called the Rabin-Karp method.

/*
Instead of checking at each position of the text if the pattern occurs or not,
it is better to check first if the contents of the current string "window"
looks like the pattern or not. In order to check the resemblance between
these two patterns, a hashing function is used. Hashing a string involves
computing a numerical value from the
value of its characters using a hash function.

The Rabin-Karp method uses the rule that if two strings are equal,
their hash values must also be equal.
Note that the converse of this statement is not always true, but a
good hash function tries to reduce the number of such hash collisions.
Rabin-Karp computes hash value of the pattern, and then goes through the
string computing hash values of all of its substrings and checking if
the pattern's hash value is equal to the substring hash value,
and advancing by 1 character every time. If the two hash values are the same,
then the algorithm verifies if the two string really are equal,
rather than this being a fluke of the hashing scheme.
It uses regular string comparison for this final check. Rabin-Karp is
an algorithm of choice for multiple pattern search.
If we want to find any of a large number, say k, fixed length patterns
in a text,
a variant Rabin-Karp that uses a hash table to check whether the hash of
a given string belongs to a set of hash values of patterns we are looking for.
Other algorithms can search for a single pattern in time order O(n),
hence they will search for k patterns in time order O(n*k).
The variant Rabin-Karp will still work in time order O(n) in the best
and average case because a hash table allows to check whether or
not substring hash equals any of the pattern hashes in time order of O(1).

Here is some code (not working though!)
*/

#include

hashing_function()
{
// A hashing function to compute the hash values of the strings.
....
}

void KarpRabinR(char *x, int m, char *y, int n)
{
int hx, hy, i, j;

printf("\nstring : [%s]"
"\nlength : [%d]"
"\npattern : [%s]"
"\nlength : [%d]\n\n", y,n,x,m);


/* Preprocessing phase */
Do preprocessing here..

/* Searching */
j = 0;
while (j <= n-m)
{
if (hx == hy && memcmp(x, y + j, m) == 0)
{
// Hashes match and so do the actual strings!
printf("\nMatch found at : [%d]\n",j);
}

hy = hashing_function(y[j], y[j + m], hy);
++j;
}
}


int main()
{

char *string="hereroheroero";
char *pattern="hero";

KarpRabin(pattern,strlen(pattern),string,strlen(string));

printf("\n\n");
return(0);

}



This is how the comparison happens visually


hereroheroero
!
hero


hereroheroero
!
hero


hereroheroero
!
hero


hereroheroero
!
hero


hereroheroero
!
hero


hereroheroero
!
hero


hereroheroero
|||| ----> Hash values match, so do the strings!
hero


hereroheroero
!
hero


hereroheroero
!
hero


hereroheroero
!
hero





Method3

The Knuth-Morris-Pratt or the Morris-Pratt algorithms are extensions of the basic Brute Force algorithm. They use precomputed data to skip forward not by 1 character, but by as many as possible for the search to succeed.

Here is some code


void preComputeData(char *x, int m, int Next[])
{
int i, j;
i = 0;
j = Next[0] = -1;

while (i < m)
{
while (j > -1 && x[i] != x[j])
j = Next[j];
Next[++i] = ++j;

}
}


void MorrisPrat(char *x, int m, char *y, int n)
{
int i, j, Next[1000];

/* Preprocessing */
preComputeData(x, m, Next);

/* Searching */
i = j = 0;
while (j < n)
{
while (i > -1 && x[i] != y[j])
i = Next[i];
i++;
j++;
if (i >= m)
{
printf("\nMatch found at : [%d]\n",j - i);
i = Next[i];
}
}
}


int main()
{
char *string="hereroheroero";
char *pattern="hero";

MorrisPrat(pattern,strlen(pattern),string,strlen(string));

printf("\n\n");
return(0);
}



This is how the comparison happens visually


hereroheroero
!
hero


hereroheroero
!
hero


hereroheroero
!
hero


hereroheroero
!
hero



hereroheroero
|||| ----> Match found!
hero


hereroheroero
!
hero




Method4

The Boyer Moore algorithm is the fastest string searching algorithm. Most editors use this algorithm.

It compares the pattern with the actual string from right to left. Most other algorithms compare from left to right. If the character that is compared with the rightmost pattern symbol does not occur in the pattern at all, then the pattern can be shifted by m positions behind this text symbol.


The following example illustrates this situation.

Example:


0 1 2 3 4 5 6 7 8 9 ...
a b b a d a b a c b a
| |
b a b a c |
<------ |
|
b a b a c


The comparison of "d" with "c" at position 4 does not match. "d" does not occur in the pattern. Therefore, the pattern cannot match at any of the positions 0,1,2,3,4, since all corresponding windows contain a "d". The pattern can be shifted to position 5. The best case for the Boyer-Moore algorithm happens if, at each search attempt the first compared character does not occur in the pattern. Then the algorithm requires only O(n/m) comparisons .

Bad character heuristics

This method is called bad character heuristics. It can also be applied if the bad character (the character that causes a mismatch), occurs somewhere else in the pattern. Then the pattern can be shifted so that it is aligned to this text symbol. The next example illustrates this situation.


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



The pattern is shifted by the longest of the two distances that are given by the bad character and the good suffix heuristics.

The Boyer-Moore algorithm uses two different heuristics for determining the
maximum possible shift distance in case of a mismatch: the "bad character"
and the "good suffix" heuristics. Both heuristics can lead to a shift distance
of m. For the bad character heuristics this is the case, if the first comparison
causes a mismatch and the corresponding text symbol does not occur in the pattern
at all. For the good suffix heuristics this is the case, if only the first
comparison was a match, but that symbol does not occur elsewhere in the pattern.
Posted by Vijay Agrawal at 2:17 PM
4 comments:

Noelani said...

    Good words.
    November 10, 2008 7:46 AM
Anonymous said...

    in 1st method some mistakes r there i think.. there it should be x(i+j)==y(i) and if(i>=n).. i think.
    verify that one
    November 19, 2008 2:32 AM
Udit said...

    Boyer�Moore string search algorithm

    #include "limits.h"
    #include "string.h"

    #define ALPHABET_SIZE (1 << CHAR_BIT)

    static void compute_prefix(const char* str, size_t size, size_t result[size]) {
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
    result[(size_t) str[i]] = i;
    }

    void prepare_goodsuffix_heuristic(const char *normal, size_t size,
    size_t result[size + 1]) {

    char *left = (char *) normal;
    char *right = left + size;
    char reversed[size+1];
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
    result[i] = size - prefix_normal[size-1];
    }

    for (i = 0; i < size; i++) {
    const size_t j = size - prefix_reversed[i];
    const size_t k = i - prefix_reversed[i]+1;

    if (result[j] > k)
    result[j] = k;
    }
    }
    /*
    * Boyer-Moore search algorithm
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
    if(haystack_len == 0)
    return NULL;
    if(needle_len == 0)
    return haystack;

    /*
    * Initialize heuristics
    */
    size_t badcharacter[ALPHABET_SIZE];
    size_t goodsuffix[needle_len+1];

    prepare_badcharacter_heuristic(needle, needle_len, badcharacter);
    prepare_goodsuffix_heuristic(needle, needle_len, goodsuffix);

    /*
    * Boyer-Moore search
    */
    size_t s = 0;
    while(s <= (haystack_len - needle_len))
    {
    size_t j = needle_len;
    while(j > 0 && needle[j-1] == haystack[s+j-1])
    j--;

    if(j > 0)
    {
    size_t k = badcharacter[(size_t) haystack[s+j-1]];
    size_t m;
    if(k < j && (m = j-k-1) > goodsuffix[j])
    s+= m;
    else
    s+= goodsuffix[j];
    }
    else
    {
    return haystack + s;
    }
    }

    return NULL; // not found
    }
#endif // FIXME

// to reverse a whole string, end should point to the last char before the NULL at its end
void strrevptr(char *beg, char *end)        // beg <= end < beg + strlen(beg)
{
    assert(beg && end && beg <= end);
	while (beg < end) {
		char c = *beg;
		*beg++ = *end;
		*end-- = c;
	}
}

static inline bool isWordEnd(char c)
{
    return (isspace(c) || c == '.' || c == ';');
}

void reverseWords(char *str)
{
    assert(str);
    char *beg = str, *end = beg;
    for (char newC, oldC = ' '; *end != NULL; end++) {
        newC = *end;
        if ( isWordEnd(newC) && ! isWordEnd(oldC) && end - beg > 1) {
            strrevptr(beg, end - 1);
            beg = end;
        } else if ( ! isWordEnd(newC) && isWordEnd(oldC)) {
            beg = end;
        }
        oldC = newC;
    }
    for ( --end; end > str; end--) {
        if ( ! isWordEnd(*end) )
            break;
    }
    if (beg < end) {
        strrevptr(beg, end);
    }
    strrevptr(str, end);
}


// Smart implementation:
int sttrevGood(char *str)
{
    assert(str);
    int len = strlen(str);
    strrevptr(str, str + len - 1);
    return len;
}



// Good implementation:
void strrevlenGood(char *str, int len)
{
     strrevptr(str, str + len - 1);
}



// Dumb implementation:
void strrevlenDumb(char *charray, int len)
{
    assert(charray && len >= 0);
	char c;
	for (int j = 0; j < len/2; j++) {
		c = charray[j];
		charray[j] = charray[len - 1 - j];
		charray[len - 1 - j] = c;
	}
}

// Dumb implementation:
void sttrevDumb(char *str)
{
    assert(str);
    int len = strlen(str);
    strrevlenDumb(str, len);
}

unsigned int lowerCaseChar(unsigned int c)
{
    if ('A' <= c && c <= 'Z') {
        return c + 'a' - 'A';
    }
    return c;
}

unsigned int upperCaseChar(unsigned int c)
{
    if ('a' <= c && c <= 'z') {
        return c + 'A' - 'a';
    }
    return c;
}

char changeCaseChar(char c)
{
    static char aLessA = 'a' - 'A';
    if ('A' <= c && c <= 'Z') {
        return c + aLessA;
    } else if ('a' <= c && c <= 'z') {
        return c - aLessA;
    }
    return c;
}

void upperCaseEvenIndexChars(char *pc)
{
    if (pc == NULL) {
        return;
    }
    for (; *pc != NULL; pc++) {
        *pc = (char)upperCaseChar(*pc);
        pc++;
        if (*pc == NULL) {
            break;
        }
    }
}


void upperCaseEveryNthChars(char *pc, int nth)
{
    if (pc == NULL) {
        return;
    }
    for (; *pc != NULL; pc++) {
        *pc = (char)upperCaseChar(*pc);
        for (int j = nth - 1; j > 0; j--) {
            if (*++pc == NULL) {            // Advance pointer and
                return;                     // return if end of string.
            }
        }
    }
}

void lowerCaseEveryMthButChangeCaseEveryNthChars(char *pc, int mth, int nth)
{
    if (pc == NULL) {
        return;
    }
    for (int m = mth, n = nth; *pc != NULL; pc++, m++, n++) {
        if (m == mth && n != nth) {
            *pc = (char)lowerCaseChar(*pc);
            m = 0;
        }
        if (n == nth) {
            *pc = changeCaseChar(*pc);
            n = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
static char * char2bins(char s[], unsigned int c)
{
    char *pc = s;
    for (int j = 8; --j >= 0; ) {
        *pc++ = (c & (1 << j)) ? '1' : '0';
    }
    return s;
}

void printRows(const char *rows[], int nRows, int nCols)
{
    for (int row = 0; row < nRows; row++) {
        const char *pRow = rows[row];
        printf("%2d  ", row);
        for (int col = 0; col < nCols; col++) {
            printf(" %c", pRow[col]);
        }
        printf("\n");
    }
}

using namespace std;

int test_map()      // (int argc, char* argv[])
{

    //typedef map<const char *, int, ltstr> mapt;
    //typedef map<string, int> mapt;
    typedef WordMap mapt;

    mapt testmap;

    testmap.insert( make_pair("fon", 3) );
    testmap.insert( make_pair("foo", 3) );
    testmap.insert( make_pair("foober", 3) );
    testmap.insert( make_pair("foobar", 3) );
    testmap.insert( make_pair("fool", 3) );
    testmap.insert( make_pair("food", 3) );
    testmap.insert( make_pair("fop", 3) );

    mapt::iterator start_it = testmap.lower_bound("____");
    mapt::iterator stop_it = testmap.upper_bound("~~~~");

    for( ; start_it != stop_it; ++start_it ) {
        cerr << (*start_it).first << '\t' << (*start_it).second << endl;
        //start_it = testmap.lower_bound("foobc");
        //stop_it = testmap.lower_bound("foo~");
    }

    //isPalindrome(lst);
    return 0;
}


static void test_bins()
{
    char bins[9] = { 0 };
    char subs[9] = { 0 };
    char xors[9] = { 0 };
    char tlds[9] = { 0 };
    for(unsigned int j = 0; j < 256; j++) {
        unsigned int c = j;
        printf("%3d  %s  %s  %s  %s\n", c, char2bins(bins, c), char2bins(subs, 255 - c), char2bins(subs, 255 - c)
            , char2bins(xors, 255 ^ c), char2bins(tlds, ~c));
    }
}

int test_charString()
{
  //const unsigned int N = 2;
	char charray[] = "This string is  8 words and 42 characters.";
	int len = strlen(charray);
	char dub[17] = "jack";

	//typedef char * (f_rpc)(void);
	//typedef f_rpc  *pf_rpc;
	//typedef pf_rpc *ppf_rpc;
 //   ppf_rpc array_of_ppf_rpc[N];
 //   pf_rpc *array_of_p_pf_rpc[N];

    printf("charray<%d>[%s]\n", len, charray);
//  upperCaseEvenIndexChars(charray+1);
//  lowerCaseEveryMthButChangeCaseEveryNthChars(charray, 3, 4);
    reverseWords(charray);
    printf("charray<%d>[%s]\n", len, charray);

    //printf("ispunct(' \"  - ( ] a) (%d %d %d %d %d %d)\n"
    //    , ispunct('\'')
    //    , ispunct('"')
    //    , ispunct('-')
    //    , ispunct('(')
    //    , ispunct('[')
    //    , ispunct('a')
    //    );

	tSLL asll[26];
	for (int j = 0; j < 25; j++) {
		asll[j].val = j + 'A';
		asll[j].nxt = &asll[j+1];
	}
	asll[25].val = 25 + 'A';
	asll[25].nxt = NULL;

	tDLL adll[26];
	for (int j = 0; j < 26; j++) {
		adll[j].val = j + 'A';
		adll[j].nxt = &adll[j+1];   // dangerous at j == 0
		adll[j].prv = &adll[j-1];   // dangerous at j == 25
	}
    adll[ 0].prv = NULL;
	adll[25].nxt = NULL;
    printDLL(&adll[0], &adll[25]);

	tSLL *rev = &asll[0];
	printSLL(rev);
	reverseSLL(&rev);
	printSLL(rev);
	rev = reverseP(rev);
	printSLL(rev);

	//func *fns[N];

	dub[5] = NULL;
    printf("dub(%s)\n", dub);

	printf("%s\n", charray);
	strrevlenGood(charray, len);
	printf("%s\n", charray);
	strrevptr(charray, charray + len - 1);
	printf("%s\n", charray);
	printf("\n");

    const char *str = "able was I ere I saw elba";
    int length = strlen(str);
    tSLL *list = string2SLL(str, length);
    printSLL(list);
    if (isPalindrome(list, length))
        printf(" is a palindrome!\n");
    else
        printf(" is NOT a palindrome!\n");
    return 0;
}
