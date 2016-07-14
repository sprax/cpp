
#include "stdafx.h"

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


int test_hour_glass_sum()
{
    vector< vector<int> > arr(6, vector<int>(6));
    for (int arr_i = 0; arr_i < 6; arr_i++){
        for (int arr_j = 0; arr_j < 6; arr_j++){
            cin >> arr[arr_i][arr_j];
        }
    }

    const int SMO = 5;      // Size Minus One
    int jon, hgs, maxHgs = INT_MIN;
    for (int j = 1; j < SMO; j++) {
        for (int k = 1; k < SMO; k++) {
            hgs = arr[j][k];
            jon = j - 1;
            hgs += arr[jon][k - 1] + arr[jon][k] + arr[jon][k + 1];
            jon = j + 1;
            hgs += arr[jon][k - 1] + arr[jon][k] + arr[jon][k + 1];
            if (maxHgs < hgs)
                maxHgs = hgs;
        }
    }
    cout << maxHgs << endl;
    return 0;
}
