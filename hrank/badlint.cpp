// bad cpp source for checking linters.   
//
// cppcheck badlint.cpp 
// Checking badlint.cpp ...
// [badlint.cpp:13]: (error) Array 'a[4]' accessed at index 4, which is out of bounds.
//
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int a[4], z = 5;
    for (int n = 0; n < z; n++)
    {
          a[n] = n;
    }
    return 0;
}
