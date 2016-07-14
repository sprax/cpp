// strUtil.cpp : utility functions for arrays
// Sprax Lines,  July 2010

#include "stdafx.h"
#include "strUtil.h"

/** This implementation of strndup is more or less like the 
 *  Libiberty version (GPL). 
 *  Beware of s == NULL, strndup does not check for that,
 *  and standard strlen will segv on NULL.
 *  Beware that if you wrap it with another function that
 *  calls strlen, your code is likely to have redundancies.
 */
char *strndup(const char *s, size_t n)
{
    char *result;
    size_t len = strlen (s);

    if (n < len)
        len = n;

    result = (char *) malloc (len + 1);
    if ( ! result)
        return 0;

    result[len] = '\0';
    return (char *) memcpy (result, s, len);
}


char *substring(const char *src, size_t startIndex, size_t subSize)
{
    // Check for NULL before calling strlen
    if (src == NULL)
        return NULL;

    // If the size of the requested substring would be <= 0, return NULL
    size_t srcSize = strlen(src);
    if (srcSize <= startIndex || subSize == 0)
        return NULL;

    // Limit size of substring to what's left from the starting index
    size_t maxSize = srcSize - startIndex;
    if (subSize > maxSize)
        subSize = maxSize;

    char * subString = (char *) malloc(subSize + 1);
    if ( ! subString)
        return NULL;

    subString[subSize] = '\0';
    return (char *)memcpy(subString, src+startIndex, subSize);
} 


char *rotateStringMod26(char *str, int rot) 
{
    if (str == NULL)
        return NULL;
    int offset = rot - 'a';
    for (char ch, *pc = str; (ch = *pc); pc++) {
        *pc = (ch + offset) % 26 + 'a';
    }
    return str;
}

///////////////////////// STRING COMPARISON for SORTING //////////////////////
//
// BEWARE: These functions take pointers to pointers to char, NOT literals or 
// the address of the first char in a char[] or char*.
// NOT: qsort_strcmp("abc", "xyz");
// NOR: char *a = "abc", char x[4] = "xyz"; qsort_strcmp(a, x);
// BUT: char *a = "abc", char x[4] = "xyz"; qsort_strcmp(&a, &x);

int qsort_strcmp(const void *pStrA, const void *pStrB)
{
    return strcmp( *(const char**)pStrA, *(const char**)pStrB);
}

int qsort_s_strcmp(void *, const void *pStrA, const void *pStrB)
{
    return strcmp( *(const char**)pStrA, *(const char**)pStrB);
}

int qsort_stricmp( const void *arg1, const void *arg2 )
{
   /* Compare all of both strings: */
   return _stricmp( * ( char** ) arg1, * ( char** ) arg2 );
}

