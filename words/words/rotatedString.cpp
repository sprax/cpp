#include "stdafx.h"
#include "tests.h"

#include<iostream>
using namespace std;

static unsigned int sgNops = 0;

/** rotated(str1, str2, length)
 *  If the non-empty str2 is a rotation of the chars in str1 (up to length), 
 *  return the non-negative offset.
 *  Otherwise, return -1.
 */
int rotated_char_naive(const char *ss, const char *tt, size_t len)
{
    int result = -1;
    char s0 = ss[0];
    for (size_t rot = 0 ; rot < len; rot++) {
        sgNops++;
        if (tt[rot] == s0) {
            size_t j;
            for (j = 1; j < len; j++) {
                sgNops++;
                if (ss[j] != tt[(rot + j) % len]) {
                    break;
                }
            }
            if (j == len) {
                result = rot;
                break;
            }
        }
    }
    return result;
}

int rotated_char_clib(const char *ss, const char *tt, size_t len)
{
    int result = -1;
    size_t size = len*2 + 1;        // 1 extra byte for NULL
    char *t2 = (char*)malloc(size*sizeof(char));
    t2[0] = NULL;
    strncat_s(t2, size, tt, len);
    strncat_s(t2, size, tt, len);
    char *ps = strstr(t2, ss);
    if (ps != NULL) {
        result = (ps - t2);
    }
    free(t2);
    return result;
}

using namespace std;

int rotated_std_string(const string &ss, const string &tt)
{
    if (ss.size() == tt.size()) {
        string t2(tt);
        t2.append(tt);
        size_t found = t2.find(ss);
        if (found != string::npos) {
            return found;
        }
    }
    return -1;
}


/** rotated(str1, str2)
 *  If the non-empty str2 is a rotation of the chars in str1, return the non-negative offset.
 *  Otherwise, return a code that's less than 0.
 */
int rotated_char(const char *ss, const char *tt)
{
    sgNops = 0;
    if ( ! ss )
        return -2;
    if ( ! tt )
        return -3;
    size_t lenss = strlen(ss);
    sgNops += lenss;
    if (lenss == 0)
        return -4;
    size_t lentt = strlen(tt);
    sgNops += lentt;
    if (lentt == 0)
        return -5;
    if (lenss != lentt)
        return -6;
    sgNops += lenss * 3;
    return rotated_char_clib(ss, tt, lenss);
}

int test_rotatedString()
{
    char *pchA = "AAAAAB";                      string strA(pchA);
    char *pchB = "AAAABA";                      string strB(pchB);
    char *pchC = "AAAAAC";                      string strC(pchC);
    char *pchD = "ABAACAAADAAAAEAAAAAFAAAAAAG";
    char *pchE = "AAAAAFAAAAAAGABAACAAADAAAAE";
    char *pchF = "AAAAAFAAAAAAGABAADAAACAAAAE";
    int status;
    status = rotated_char(pchA, pchB);
    printf("rotated_char_clib got %d in %u ops from <%s, %s>\n", status, sgNops, pchA, pchB);
    status = rotated_char(pchA, pchC);
    printf("rotated_char_clib got %d in %u ops from <%s, %s>\n", status, sgNops, pchA, pchC);
    sgNops = 0;
    status = rotated_std_string(strA, strB);
    printf("rotated_str_string got %d in %u ops from <%s, %s>\n", status, sgNops, strA.c_str(), strB.c_str());
    status = rotated_std_string(strA, strC);
    printf("rotated_str_string got %d in %u ops from <%s, %s>\n", status, sgNops, pchA, pchC);

    return 0;
}