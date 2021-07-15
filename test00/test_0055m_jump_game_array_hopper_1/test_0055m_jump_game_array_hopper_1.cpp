// @file: test_1059m_all_paths_from_source_lead_to_destination.cpp
// @auth: Sprax Lines
// @date: 2021-06-22 20:22:18 Tue 22 Jun

/** Example build commands:
g++ -I../../include -Wall -Wextra -std=gnu++14 -pthread test_*.cpp -lgtest -lgtest_main -o tmp.out && ./tmp.out
*/
#include "std_gtest.h"
using namespace std;

/**
55. Jump Game.  Medium

Given an array of non-negative integers,
you are initially positioned at the first index of the array.

Each element in the array represents your maximum jump length at that position.

Determine if you are able to reach the last index.

Example 1:
Input:  {2, 3, 1, 1, 4}
Output: True
Logic:  Jump 1 step from index 0 to 1, then 3 steps to the last index.

Example 2:
Input:  {3, 2, 1, 0, 4}
Output: False
Logic:  You will always arrive at index 3 no matter what.  Its maximum
             jump length is 0, which makes it impossible to reach the last
             index.

Accepted 407853 / 1208037 = 33.8%

class SolutionShort:
    '''
    Simple and pretty optimal.

    Runtime: 80 ms, faster than 92.42% in Python3.
    Memory Usage: 16.4 MB, less than 6.35% in Python3.

    Runtime: 512 ms, faster than 43.99% in Python3.
    Memory Usage: 15.3 MB, less than 39.27% in Python3.
    '''

    def canJump(self, nums: List[int]) -> bool:
        max_idx = 0
        for idx in range(len(nums)):
            if idx > max_idx:       # Exceeded the max_idx?
                return False        # Then we could not have jumped here.
            max_idx = max(max_idx, idx + nums[idx])
        return True
*/

class SolutionBase
{
public:
    virtual
    bool canJump(vector<int>& nums)
    = 0;
};

/**
55. Jump Game.  Medium
Linear, greedy, non-optimal:

IDEA: Observe that the last position is reachable
    IFF nums[pos] + pos >= last_pos for some pos < last_pos,
    which in turn is reachable IFF nums[lower_pos] + lower_pos >= pos,
    and so forth, which is iterable.
NOTE: Every position is visited exactly once.
A greedier forward-hopping algo might skip some steps, but fail.

Runtime: 36 ms, faster than 94.35% of C++ online submissions for Jump Game.
Memory Usage: 48.3 MB, less than 38.01% of C++ online submissions for Jump Game.
 */
class SolutionGreedyBackward : public SolutionBase
{
public:
    /** the answer method */
    bool canJump(vector<int>& nums) {
        auto size = nums.size();

        if (size < 2) {
            return true;    // 'last index' is -1 or 0.
        }
        auto hi_pos = size - 1;
        auto lo_pos = hi_pos;

        while (lo_pos > 0) {
            lo_pos -= 1;
            if (lo_pos + nums[lo_pos] >= hi_pos) {
                hi_pos = lo_pos;
            }
        }
        return hi_pos == lo_pos;
    }
};


class SolutionEarlyReturn : public SolutionBase
{
    bool canJump(vector<int>& nums) {
        int size = nums.size();

        if (size < 2) {
            return true;    // 'last index' is -1 or 0.
        }
        auto last    = size - 1;
        auto max_idx = 0;

        for (auto idx = 0; idx < size; idx++) {
            if (idx > max_idx) {    //  Exceeded the max_idx?
                return false;       // Then we could not have jumped here.
            }
            max_idx = max(max_idx, idx + nums[idx]);
            if (max_idx >= last) {
                return true;
            }
        }
        return false;
    }
};

///////////////////////////////////////////////////////////////////////////////

class Solution : public SolutionGreedyBackward { };
// class Solution : public SolutionEarlyReturn { };

///////////////////////////////////////////////////////////////////////////////

namespace { // anonymous
vector<pair<vector<int>, int> > tpairs {
    { { 2, 3, 1, 1, 4 }, true },
    { { 3, 2, 1, 0, 4 }, false },
};

/// Expected: All test pairs pass.
void test_soln(SolutionBase&soln) {
    for (auto& tpair : tpairs) {
        auto actual = soln.canJump(tpair.first);
        EXPECT_EQ(actual, tpair.second);
    }
}
}   // anonymous namespace

TEST(SolutionGreedyBackward, canJump)
{
    SolutionGreedyBackward soln;

    test_soln(soln);
}

TEST(SolutionEarlyReturn, canJump)
{
    SolutionEarlyReturn soln;

    test_soln(soln);
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
