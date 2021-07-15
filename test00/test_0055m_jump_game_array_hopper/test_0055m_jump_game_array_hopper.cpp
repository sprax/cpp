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
'''


class SolutionDpExtraSpaceTLE:

    def canJump(self, nums: List[int]) -> bool:
        '''
        55. Jump Game.  Medium
        dynamic programming with auxiliary array, bottom-up.
        Return on first unreachable position or on
        first solution, not necessarily with min jumps.
        Space O(N), but
        Time O(N*A), where A = avg(nums).  This *can* be worse than O(N^2)
        TIME LIMIT EXCEEDED on large test case.
        '''
        size = len(nums)
        if size < 2:
            return True  # 'last index' is -1 or 0.
        hops = [None] * size
        hops[0] = 0
        for pos, hop in enumerate(hops):
            if hop is None:
                return False
            max_adv = nums[pos]
            for adv in range(1, max_adv + 1):
                new_pos = pos + adv
                if (new_pos >= size - 1):
                    return True
                hops[new_pos] = pos   # any value other than None
        return True

###############################################################################


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

###############################################################################


class SolutionEarlyReturn:
    '''
    Simple and seems optimal (on LeetCode test cases).

    Runtime: 72 ms, faster than 99.41% in Python3.
    Memory Usage: 16.4 MB, less than 6.35% in Python3.

    Runtime: 468 ms, faster than 79.51% in Python3.
    Memory Usage: 15.4 MB, less than 17.58% in Python3.
    '''

    def canJump(self, nums: List[int]) -> bool:

        size = len(nums)
        last = size - 1
        max_idx = 0

        for idx in range(len(nums)):
            if idx > max_idx:       # Exceeded the max_idx?
                return False        # Then we could not have jumped here.
            max_idx = max(max_idx, idx + nums[idx])
            if max_idx >= last:
                return True
        return True

###############################################################################


SOLNS = {
    SolutionDpExtraSpaceTLE,
    SolutionGreedyBackward,
    # SolutionShort,
    # SolutionEarlyReturn,
}


class Solution(SOLNS[-1]):
    pass

###############################################################################


class TestSolution(LeetTestCase):
    ''' unit tests for Solution class '''

    def __init__(self, *args, **kwargs):
        super(TestSolution, self).__init__(*args, **kwargs)

    def driver(self, solute=None, verbose=2, verify_f=is_wrong, equals_f=None):
        ''' unit test driver for Solution classes
    '''
        solute = Solution() if solute is None else solute
        method = solute.canJump

        sample = {2, 3, 1, 1, 4}
        expect = True
        self.verify(method, sample, expect, verbose, verify_f)

        sample = {3, 2, 1, 0, 4}
        expect = False
        self.verify(method, sample, expect, verbose, verify_f)

        self.report(solute.__class__.__name__, method, verbose)

    def test_solutions(self, verbose=2):
        ''' unit tests for named solution classes '''
        for soln in SOLNS:
            self.driver(soln(), verbose)
        self.finish(__file__, None, verbose, asserts=0)


if __name__ == '__main__':
    TestSolution().test_solutions()
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
///////////////////////////////////////////////////////////////////////////////

class Solution : public SolutionGreedyBackward { };
// class Solution : public SolutionFindLastOf { };

///////////////////////////////////////////////////////////////////////////////

static vector<pair<vector<int>, int> > tpairs {
    { { 2, 3, 1, 1, 4 }, true },
    { { 3, 2, 1, 0, 4 }, false },
};


// ß Expected: TODOC.
TEST(SolutionGreedyBackward, canJump)
{
    auto solute = new SolutionGreedyBackward();

    for (auto& tpair : tpairs) {
        auto actual = solute->canJump(tpair.first);
        EXPECT_EQ(actual, tpair.second);
    }
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
