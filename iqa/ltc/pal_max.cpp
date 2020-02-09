/// file: pal_max.cc
/** Example build commands:
clang++ -std=c++11 pal_max.cpp -o tmp.out && tmp.out
    g++ -std=c++14 pal_max.ccp -o tmp.out && tmp.out
*/

#include <chrono>         // std::chrono::str
#include <iostream>       // std::cout
#include <string>         // std::thread, std::this_thread::sleep_for
#include <thread>         // std::thread, std::this_thread::sleep_for

using namespace std;

class Solution {
public:

    size_t odd_pal_size(const string& str, size_t idx, size_t end_idx) {
        size_t j = 1;
        for (; j <= end_idx; j++) {
            if (str[idx - j] != str[idx + j])
                break;
        }
        return (j - 1)*2 + 1;
    }

    size_t evn_pal_size(const string& str, size_t idx, size_t end_idx) {
        size_t j = 1;
        for (; j <= end_idx; j++) {
            if (str[idx - j] != str[idx + j - 1])
                break;
        }
        return (j - 1)*2;
    }

    string longestPalindrome(string str)
    {
        size_t size = str.size();
        if (size < 2) {
            if (size == 1) {
                return str;
            }
            return "";
        }
        size_t pal_max = 0; // result to return
        size_t evn_idx = 0, odd_idx = 0;
        for (size_t j = 1; j < size; j++) {
            size_t end_idx = std::min(j, size - j);
            size_t evn_max = evn_pal_size(str, j, end_idx);
            if (pal_max < evn_max) {
                pal_max = evn_max;
                evn_idx = j;
            }
            size_t odd_max = odd_pal_size(str, j, end_idx);
            if (pal_max < odd_max) {
                pal_max = odd_max;
                odd_idx = j;
            }
        }
        if (pal_max % 2) {  // odd
            cerr << "odd pal_max: " << pal_max << " at odd_idx: " << odd_idx << endl;
            return str.substr(odd_idx - pal_max/2, pal_max);
        } else {
            cerr << "even pal_max: " << pal_max << " at evn_idx: " << evn_idx << endl;
            return str.substr(evn_idx - pal_max/2, pal_max);
        }
    }


    size_t pal_size(const string& str, int lef_idx, int rgt_idx)
    {
        for (; str[lef_idx] == str[rgt_idx] ;) {
            if (--lef_idx < 0) {
                return rgt_idx - lef_idx;
            }
            if (++rgt_idx >= str.size()) {
                return rgt_idx - lef_idx - 1;
            }
        }
        return rgt_idx - lef_idx + 1;
    }

    string longest_palindrome(string str)
    {
        size_t size = str.size();
        if (size < 2) {
            if (size == 1) {
                return str;
            }
            return "";
        }
        size_t pal_max = 0; // result to return
        size_t evn_idx = 0, odd_idx = 0;
        for (size_t j = 1; j < size; j++) {
            size_t end_idx = std::min(j, size - j);
            size_t evn_max = evn_pal_size(str, j, end_idx);
            if (pal_max < evn_max) {
                pal_max = evn_max;
                evn_idx = j;
            }
            size_t odd_max = odd_pal_size(str, j, end_idx);
            if (pal_max < odd_max) {
                pal_max = odd_max;
                odd_idx = j;
            }
        }
        if (pal_max % 2) {  // odd
            cerr << "odd pal_max: " << pal_max << " at odd_idx: " << odd_idx << endl;
            return str.substr(odd_idx - pal_max/2, pal_max);
        } else {
            cerr << "even pal_max: " << pal_max << " at evn_idx: " << evn_idx << endl;
            return str.substr(evn_idx - pal_max/2, pal_max);
        }
    }

};

int main(int argc, const char * argv[])
{
    string ans, str("badad");
    Solution sol;
    ans = sol.longestPalindrome(str);
    cerr << ans << endl;
    ans = sol.longestPalindrome("");
    cerr << ans << endl;
    ans = sol.longestPalindrome("1");
    cerr << ans << endl;
    ans = sol.longestPalindrome("22");
    cerr << ans << endl;
    ans = sol.longestPalindrome("333");
    cerr << ans << endl;
    ans = sol.longestPalindrome("4444");
    cerr << ans << endl;
    ans = sol.longestPalindrome("12354345123");
    cerr << ans << endl;
    ans = sol.longestPalindrome("126544567761");
    cerr << ans << endl;
}
