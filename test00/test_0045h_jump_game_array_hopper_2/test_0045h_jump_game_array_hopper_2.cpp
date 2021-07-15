// @file: test_0045h_jump_game_array_hopper_2.cpp
// @auth: Sprax Lines
// @date: 2021-06-01 12:25:21 Tue 01 Jun

/// file: test_0001e_two_num_sum.cpp

/** Example build commands:
    # OSX:
clang++ -std=c++17 -I../../test -lgtest test_0001e_two_num_sum.cpp -o tmp.out
&& tmp.out

# One command with no saved .o file (c++ or g++ on Linux, but not cc, nor cpp,
and the pthread is needed):
g++ -I../../include -Wall -std=gnu++14 -pthread
test_0045h_jump_game_array_hopper_2.cpp  -lpthread  -lgtest -lgtest_main -o
tmp.out && ./tmp.out
*/
#include "std_gtest.h"
using namespace std;


/**
45. Jump Game II.  Medium
    Up: 4241  Down: 183

Given an array of non-negative integers nums, you are initially positioned at
the first index of the array.

Each element in the array represents your maximum jump length at that position.

Your goal is to reach the last index in the minimum number of jumps.

You can assume that you can always reach the last index.

Example 1:
Input:  nums = [2, 3, 1, 1, 4]
Output: 2
Logic:  The minimum number of jumps to reach the last index is 2. Jump 1 step from index 0 to 1, then 3 steps to the last index.

Example 2:
Input:  nums = [2, 3, 0, 1, 4]
Output: 2

Constraints:
1 <= nums.length <= 1000
0 <= nums[i] <= 105

Accepted 347808 / 1075335 = 32.3%  (circa 2021-06-01)
*/


class SolutionBase
{
public:
    virtual
    int jump(vector<int>& nums) // min number of jumps
    = 0;
};

/**
Greedy algorithm checking feasibility only at the end.

    Runtime: 12 ms, faster than 75.56% in C++.
    Memory Usage: 16.3 MB, less than 74.31% in C++.
*/
class SolutionGreedy : public SolutionBase
{
public:
    int jump(vector<int>& nums) {
        size_t min_jumps = 0, jump_end = 0, max_idx = 0;
        size_t last_idx = nums.size() - 1;

        for (size_t idx = 0; idx < last_idx; idx++) {
            max_idx = max(max_idx, idx + nums[idx]);
            if (idx == jump_end) {
                jump_end = max_idx;
                min_jumps++;
            }
        }
        return (max_idx >= last_idx) ? min_jumps : -1;
    }
};

/**
JS, Python, Java, C++ | Simple O(1) Space Solution w/ Explanation
Based on code by sgallivan (views -; votes 29; reputation -)
2798
2798
Last Edit: 11 hours ago

669 VIEWS

(Note: This is part of a series of Leetcode solution explanations.
If you like this solution or find it useful, please upvote this post.)

Idea:
Since each element of our input array (N) represents the maximum jump length
and not the definite jump length, that means we can visit any index between the
current index (i) and i + N[i]. Stretching that to its logical conclusion,
we can safely iterate through N while keeping track of the furthest index
reachable (next) at any given moment (next = max(next, i + N[i])).
We'll know we've found our solution once next reaches or passes the last index
(next >= N.length - 1).

The difficulty then lies in keeping track of how many jumps it takes to reach
that point.  We can't simply count the number of times we update next,
as we may see that happen more than once while still in the current jump's
range.  In fact, we can't be sure of the best next jump until we reach the end
of the current jump's range.

So in addition to next, we'll also need to keep track of the current jump's
endpoint (curr) as well as the number of jumps taken so far (ans).

Since we'll want to return ans at the earliest possibility,
we should base it on next, as noted earlier.  With careful initial definitions
for curr and next, we can start our iteration at i = 0 and ans = 0 without the
need for edge case return expressions.

Time Complexity: O(N) where N is the length of N
Space Cmplexity: O(1)
Implementation:
There are only minor differences in the code of all four languages.

Javascript Code:
The best result for the code below is 72ms / 37.7MB (beats 95% / 100%).

var jump = function(N) {
    let len = N.length - 1, curr = -1, next = 0, ans = 0
    for (let i = 0; next < len; i++) {
        if (i > curr) ans++, curr = next
        next = Math.max(next, N[i] + i)
    }
    return ans
};
Python Code:
The best result for the code below is 16ms / 14.0MB (beats 100% / 99%).

class Solution:
    def jump(self, N: List[int]) -> int:
        Nlen, curr, nxt, ans, i = len(N) - 1, -1, 0, 0, 0
        while nxt < Nlen:
            if i > curr:
                ans += 1
                curr = nxt
            nxt = max(nxt, N[i] + i)
            i += 1
        return ans
Java Code:
The best result for the code below is 0ms / 36.1MB (beats 100% / 85%).

class Solution {
    public int jump(int[] N) {
        int len = N.length - 1, curr = -1, next = 0, ans = 0;
        for (int i = 0; next < len; i++) {
            if (i > curr) {
                ans++;
                curr = next;
            };
            next = Math.max(next, N[i] + i);
        };
        return ans;
    };
};
C++ Code:
The best result for the code below is 0ms / 7.9MB (beats 100% / 100%).

    Runtime: 16 ms, faster than 57.93% in C++.
    Memory Usage: 16.3 MB, less than 50.48% in C++.
*/
class SolutionSloppy : public SolutionBase
{
public:
    int jump(vector<int>& N) {
        int len = N.size() - 1, curr = -1, next = 0, ans = 0;

        for (int i = 0; next < len; i++) {
            if (i > curr) {
                ans++, curr = next;
            }
            next = max(next, N[i] + i);
        };
        return ans;
    }
};

///////////////////////////////////////////////////////////////////////////////

// class Solution : public SolutionGreedy { };
class Solution : public SolutionSloppy { };

///////////////////////////////////////////////////////////////////////////////

namespace { // anonymous
vector<pair<vector<int>, int> > tpairs {
    { { 2, 3, 1, 1, 4}, 2 },
    { { 2, 3, 1, 1, 4 }, 2 },
    // { { 3, 2, 1, 0, 4 }, -1 },   // invalid because end is unreachable
};

/// Expected: All test pairs pass.
void test_soln(SolutionBase&soln) {
    for (auto& tpair : tpairs) {
        auto sample(tpair.first);
        auto actual = soln.jump(sample);
        EXPECT_EQ(actual, tpair.second);
    }
}
}   // anonymous namespace


TEST(SolutionGreedy, jump)
{
    SolutionGreedy soln;

    test_soln(soln);
}

TEST(SolutionSloppy, jump)
{
    SolutionSloppy soln;

    test_soln(soln);
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv, char **envp)
{
    if (argc > 1) {
        while (*envp) {
            printf("%s\n", *envp++);
        }
    }
    //  tru::TestYesNoPrompt(5);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
