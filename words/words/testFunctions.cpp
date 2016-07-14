// testFunctions.cpp

#include "stdafx.h"
#include "tests.h"

typedef bool (*boolFunc_1int)(int);
typedef bool (*boolFunc_2int)(int,int);

int test_boolFunctionOneInt_MinMax( boolFunc_1int boolFunc, int beg, int end)
{
    int count = 0;
    for (int j = beg; j < end; j++) {
        if (boolFunc(j)) {
            count++;
            printf(" %2d", j);
        }
    }
    puts("\n");
    return count;
}

int test_boolFunctionTwoInt_MinMax( boolFunc_2int boolFunc, int beg, int end)
{
    int count = 0;
    for (int j = beg; j < end; j++) {
        for (int k = beg; k < end; k++) {
            if (boolFunc(j,k)) {
                count++;
                printf(" (%2d %2d)", j, k);
            }
        }
    }
    puts("\n");
    return count;
}
