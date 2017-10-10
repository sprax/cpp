
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;


int main(){
    int n;
    cin >> n;
    const char *outs[] = { "Greater than 9", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    if (0 < n && n < 10) {
        cout << outs[n] << endl;        
    } else {
        cout << outs[0] << endl;
    }
    return 0;
}

