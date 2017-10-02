// bad cpp source for checking linters.
//
// cppcheck badlint.cpp
// Checking badlint.cpp ...
// [badlint.cpp:13]: (error) Array 'a[4]' accessed at index 4, which is out of bounds.
//
#include "time.h"
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

bool changed(long& changeMe) {
    srandom(changeMe); // without different seeds to srandom, random will produce the same sequence
    long rando = random();
    if (changeMe != rando) {
        changeMe = rando;
        return true;
    }
    return false;
}

int main() {
    // int a[4], z = 5;
    // for (int n = 0; n < z; n++)
    // {
    //       a[n] = n;
    // }
    time_t seed = time(NULL);
    cout << "seed is " << seed << " of type time_t and size " << sizeof(time_t) << endl;
    cout << "sizeof(long) is " << sizeof(long) << endl;
    cout << "time_t seed = time(NULL); ==> " << seed << endl;
    int dif = changed(seed);
    cout << "changed returned " << dif << " and seed is now " << seed << endl;
    return 0;
}
