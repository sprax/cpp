
// charString.cpp : functions on C-style strings: NULL-terminated char arrays
//

#include "stdafx.h"

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
void sttrevDumb(char *str)
{
    assert(str);
    int len = strlen(str);
    strrevlenDumb(str, len);
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

void test_charString() 
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
	asll[25].nxt = NULL;

	//tDLL adll[27];
	for (int j = 0; j < 26; j++) {
		asll[j].val = j + 'A';
		asll[j].nxt = &asll[j+1];
	}
	asll[25].nxt = NULL;

	tSLL *rev = &asll[0];
	printtSLL(rev);
	reverseSLL(&rev);
	printtSLL(rev);
	rev = reverseP(rev);
	printtSLL(rev);

	//func *fns[N];

	dub[5] = NULL;
    printf(dub);

	charray[0] = 'Z';
	printf("%s\n", charray);
	strrevlenGood(charray, len);
	printf("%s\n", charray);
	strrevptr(charray, charray + len - 1);
	printf("%s\n", charray);
}






////////////////////////////////////////////////////////////////////////////////////////////


static char * char2bins(char s[], unsigned char c)
{
    char *pc = s;
    for (int j = 8; --j >= 0; ) {
        *pc++ = (c & (1 << j)) ? '1' : '0';
    }
    return s;
}







using namespace std;

int test_map()      // (int argc, char* argv[])
{

    //typedef map<const char *, int, ltstr> mapt;
    //typedef map<string, int> mapt;
    typedef MapSL mapt;

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
