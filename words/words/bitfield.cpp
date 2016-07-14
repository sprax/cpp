#include "stdafx.h"

// FIXME: this bitvector thing seems stupid; it's redundant with positions array

using namespace std;

typedef unsigned int unt;

void setbit(unt *bitvector, unt bit)
{
    *bitvector = *bitvector |(1<<bit);
}

int getbit(const unt bitvector, unt bit)
{
    unt shifted = 1<<bit;
    unt vector  = bitvector & (1<<bit);
    return (bitvector & (1<<bit)) >> bit;
}

int test_bitvector()
{
    unt num = 1 + 4 + 16 + 64 + 256;   // dec(341) = bin(101010101)
    int b4 = getbit(num, 5);
    setbit(&num, 5);
    b4 = getbit(num, 5);
    int shiftPos =  63 >> 3;
    int shiftNeg = -63 >> 3;
    return 0;
}
