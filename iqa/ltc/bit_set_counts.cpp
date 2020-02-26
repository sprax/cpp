/// file: pal_max.cc
/** Example build commands:
clang++ -std=c++11 pal_max.cpp -o tmp.out && tmp.out
    g++ -std=c++14 pal_max.ccp -o tmp.out && tmp.out
*/

#include <chrono>         // std::chrono::str
#include <iostream>       // std::cout
#include <string>         // std::thread, std::this_thread::sleep_for
#include <vector>

using namespace std;

static void show_vec(const vector<int>& ivec) {
    for (int j : ivec) {
        cerr << j << " ";
    }
    cerr << endl;
}

class Solution {
    //               0   2   4       8      12    15
    // vector<int> bcs_{0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4
    //             //  16              24            31
    //                 ,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5
    //             //  32              40            47
    //                 ,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5
    //             //  48              56            63
    //                 ,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6
    //                 ,1
    // };
    // int po2_ = 64;

    //               0   2   4       8      12    15
    vector<int> bcs_{0,1,1,2,1,2,2,3,1,
    };
    int po2_ = 8;

public:
    vector<int> countBits(int num) {
        size_t count = num + 1;
        auto beg = bcs_.begin();
        while (po2_ < count) {
            auto half = std::vector<int>(bcs_.end() - po2_/2, bcs_.end());
            bcs_.insert(bcs_.end(), half.begin(), half.end());
            bcs_[po2_ + po2_/2] += 1;   // fix up the end now; midpoint of all to be appended.
            for (auto& elt : half) {
                elt += 1;
            }
            bcs_.insert(bcs_.end(), half.begin(), half.end());
            po2_ += po2_;
            bcs_[po2_] = 1;
        }
        if (count > bcs_.size()) {
            throw std::runtime_error("count not yet stored: " + std::to_string(count));
        }
        beg = bcs_.begin();
        return std::vector<int>(beg, beg + count);
    }

    void show_bcs() { show_vec(bcs_); }

};

int main(int argc, const char * argv[])
{
    Solution sol;
    vector<int> ans;

    ans = sol.countBits(5);
    show_vec(ans);
    sol.show_bcs();

    ans = sol.countBits(9);
    show_vec(ans);
    sol.show_bcs();

    ans = sol.countBits(16);
    show_vec(ans);
    sol.show_bcs();
}
