// fibonacci.cpp

#include "stdafx.h"

#include <map>
#include <unordered_map>
using namespace std;
 
unsigned long long fibmem_fixed_array(int n)
{
    const int size = 94;    // fib(93) == 12200160415121876738 is biggest f.n. < ULLONG_MAX
    static long long fmem[size] = { 0, 1, 1, 0, };
    assert(0 < n && n < 94);
    if (n < size) {
        if (fmem[n] > 0) {
            return fmem[n];
        } else {
            return fmem[n] = fibmem_fixed_array(n-1) + fibmem_fixed_array(n-2);
        }
    }
    return 0;
}

unsigned long  fibmem(int n)
{
    static int size = 0;
    static int *fmem = NULL;
    assert(n > 0);
    if (n < 3) {
        return 1;
    }
    if (size <= n) {
        int *temp = (int*)calloc(n*2 + 64, sizeof(unsigned long));
        if (fmem != NULL) {
            memcpy(temp, fmem, size*sizeof(unsigned long ));
            free(fmem);
        }
        size = n*2 + 64;
        fmem = temp;
    }
    if (fmem[n] > 0) {
        return fmem[n]; 
    } else {
        return fmem[n] = fibmem(n-1) + fibmem(n-2);
    }
}



unsigned long long fibmemo(int n)
{
    static unordered_map<int,unsigned long long> fmemo;
    //assert(n > 0);
    // liberal definition for n < 1:
    if (n < 1)
        return 0;
    if (n < 3) 
        return 1;
        if (n > 93) 
                return 0; // ULLONG_MAX;
    if (fmemo.count(n) == 0) {
        return fmemo[n] = fibmemo(n-1) + fibmemo(n-2);
    } 
    return fmemo.at(n);
}
 
void test_fibmem()
{
    for (int j = 1; j < 55; j++) {
        cout << "fibmem(" << j << ") is: " << fibmem(j) << endl;
    }
}
 
int test_fibmemo()
{
    for (int j = 1; j < 100; j++) {
        cout << "fibmemo(" << j << ") is: " << fibmemo(j) << endl;
    }
    return 0;
}

int test_fibonacci()
{
    test_fibmem();
    test_fibmemo();
    return 0;
}