// strUtil_h : common functions for strings represented as char * or char []
// Sprax Lines,  July 2010

#ifndef strUtil_hpp
#define strUtil_hpp

/** This implementation of strndup is more or less like the 
 *  Libiberty version (GPL). 
 *  Beware of s == NULL, strndup does not check for that,
 *  and standard strlen will segv on NULL.
 *  Beware that if you wrap it with another function that
 *  calls strlen, your code is likely to have redundancies.
 */
char *strndup(  const char *src, size_t len);

char *substring(const char *src, size_t startIndex, size_t subSize);

char *rotateStringMod26(char *str, int rot) ;

// BEWARE: These functions take pointers to pointers to char, NOT literals or the 
// address of the first char in a char[] or char*.
// NOT: qsort_strcmp("abc", "xyz");
// NOR: char *a = "abc", char x[4] = "xyz"; qsort_strcmp(a, x);
// BUT: char *a = "abc", char x[4] = "xyz"; qsort_strcmp(&a, &x);

typedef int (__cdecl *qsort_s_cmp )(void *, const void *, const void *);
int qsort_s_strcmp(void *, const void *pStrA, const void *pStrB);

int qsort_strcmp(const void *pStrA, const void *pStrB);
int qsort_stricmp(const void *pStrA, const void *pStrB);

#endif // strUtil_hpp
