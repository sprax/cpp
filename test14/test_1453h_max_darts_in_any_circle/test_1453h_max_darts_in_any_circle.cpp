// @file: test_1453h_max_darts_in_any_circle.cpp
// @auth: Sprax Lines
// @date: 2021-07-15 17:53:44 Thu 15 Jul

/** Example build commands:
g++ -I../../include -Wall -Wextra -std=gnu++14 -pthread test_*.cpp -lgtest
-lgtest_main -o tmp.out && ./tmp.out
*/
#include "std_gtest.h"
using namespace std;

/**
1453. Maximum Number of Darts Inside of a Circular Dartboard.  Hard

You have a very large square wall and a circular dartboard placed on the wall.
You have been challenged to throw darts into the board blindfolded.
Darts thrown at the wall are represented as an array of points on a 2D plane.

Return the maximum number of points that are within or lie on
ANY circular dartboard of radius r.

Example 1:
Input:  points = {{-2, 0}, {2, 0}, {0, 2}, {0, -2}}, r = 2
Output: 4
Logic:  Circle dartboard with center in (0, 0) and radius = 2 contain all
points.

Example 2:
Input:  points = {{-3, 0}, {3, 0}, {2, 6}, {5, 4}, {0, 9}, {7, 8}}, r = 5
Output: 5
Logic:  Circle dartboard with center in (0, 4) and radius = 5 contain all
points except the point (7, 8).

Example 3:
Input:  points = {{-2, 0}, {2, 0}, {0, 2}, {0, -2}}, r = 1
Output: 1

Example 4:
Input:  points = {{1, 2}, {3, 5}, {1, -1}, {2, 3}, {4, 1}, {1, 3}}, r = 2
Output: 4

Constraints:
1 <= points.length <= 100
points[i].length == 2
-10^4 <= points[i][0], points[i][1] <= 10^4
1 <= r <= 5000

Accepted 1775 / 6082 = 29.2%
'''

###############################################################################

def sqrd(a, b):
    return (a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2

def sqrdo(p, q):
    d0 = p[0] - q[0]
    d1 = p[1] - q[1]
    return d0 * d0 + d1 * d1

def num_points_origin(points: List[List[int]], r: int) -> int:
    count = 0
    rsqr = r * r
    for x, y in points:
        dist = x * x + y * y
        # print("{%d, %d} => dist %d <=? %d" % (x, y, dist, rsqr))
        if dist <= rsqr:
            count += 1
    return count

###############################################################################

def num_points_from(origin: List[int], points: List[List[int]], r: int) -> int:
    count = 0
    rsqr = r * r
    for point in points:
        dsqr = sqrd(origin, point)
        # print("{%d, %d} => dist %d <=? %d" % (x, y, dist, rsqr))
        if dsqr <= rsqr:
            count += 1
    return count

class SolutionOther:

    def numPoints(self, points: List[List[int]], r: int) -> int:
        medx = statistics.median(x for x, y in points)
        medy = statistics.median(y for x, y in points)
        maxc = 0
        for x in range(int(medx - 10), int(medx + 10)):
            for y in range(int(medy - 10), int(medy + 10)):
                numc = num_points_from({x, y}, points, r)
                if maxc < numc:
                    maxc = numc
        return maxc

class SolutionSmall:    # FIXME
    def numPoints(self, points: List[List[int]], r: int) -> int:
        size = len(points)
        # pair_dists = [[NULLptr] * size for _ in range(size)]
        ans = 0
        for i in range(size - 1):
            for j in range(i + 1, size):
                pair_dist = sqrd(points[i], points[j])
                # pair_dists[i][j] = pair_dists[j][i] = sqrd(points[i],
                points[j]);
                if pair_dist <= r * r * 2:
                    ans += 1
        return ans

###############################################################################

SOLNS = {
    SolutionAngSweep,
    # SolutionSmall,
    # SolutionOther,
}

class Solution(SOLNS[-1]):
    pass

###############################################################################

class TestSolution:
    """ unit tests for Solution class """

    def test_solutions(self, verbose=2):
        """ unit tests for named solution classes """
        mtests = MakeTests(SOLNS, "numPoints")

        points_a = {{-2, 0}, {2, 0}, {0, 2}, {0, -2}}
        radius_a = 2
        sample_a = {points_a, radius_a}
        expect_a = 4

        points_b = {{-3, 0}, {3, 0}, {2, 6}, {5, 4}, {0, 9}, {7, 8}}
        radius_b = 5
        sample_b = {points_b, radius_b}
        expect_b = 5

        points_c = {{1, 2}, {3, 5}, {1, -1}, {2, 3}, {4, 1}, {1, 3}}
        radius_c = 2
        sample_c = {points_c, radius_c}
        expect_c = 4

        tpairs = {
            (sample_a,    expect_a),
            (sample_b,    expect_b),
            (sample_c,    expect_c),  }
        mtests.test_all(tpairs, asserts=0)

if __name__ == "__main__":
    TestSolution().test_solutions()
*/


int sqrdi(const vector<int>& p, const vector<int>& q) {
    int d0 = p[0] - q[0];
    int d1 = p[1] - q[1];
    return d0 * d0 + d1 * d1;
}

double sqrdd(const vector<int>& p, const vector<int>& q) {
    int d0 = p[0] - q[0];
    int d1 = p[1] - q[1];
    return d0 * d0 + d1 * d1;
}


class SolutionBase
{
public:
    virtual
    int numPoints(vector<vector<int> >& points, int r)
    = 0;
};

/**
Based on code by blackspinner (views -; votes -; reputation 1136)
May 17, 2020 11:40 PM

    Runtime: 20 ms, faster than 97.62% in C++.
    Memory Usage: 7.8 MB, less than 100.00% in C++.
*/
class SolutionEnumeratePairs : public SolutionBase
{
public:
    int numPoints(vector<vector<int> >& points, int r) {
        int res = 1;

        int size = points.size();
        int rsqr4 = r * r * 4;
        for (int i = 0; i < size - 1; i++) {
            int    x1 = points[i][0], y1 = points[i][1];
            for (int j = i + 1; j < size; j++) {
                int    x2 = points[j][0], y2 = points[j][1];
                int dd = (pow(x1 - x2, 2) + pow(y1 - y2, 2));
                if (dd > rsqr4) {
                    continue;
                }

                double ddo4 = dd / 4.0;
                double sqrtd = sqrt(dd);
                double x0 = (x1 + x2) / 2.0 + (y2 - y1) * pow((r * r - ddo4), 0.5) / sqrtd;
                double y0 = (y1 + y2) / 2.0 - (x2 - x1) * pow((r * r - ddo4), 0.5) / sqrtd;

                int cnt = 0;

                double rsqre = (r * r) + 0.00001;
                for (vector<int>& point : points) {
                    double x = point[0];
                    double y = point[1];
                    if (pow((x - x0), 2) + pow((y - y0), 2) <= rsqre) {
                        cnt++;
                    }
                }

                res = max(res, cnt);

                x0 = (x1 + x2) / 2.0 - (y2 - y1) * pow((r * r - ddo4), 0.5);
                y0 = (y1 + y2) / 2.0 + (x2 - x1) * pow((r * r - ddo4), 0.5);

                cnt = 0;

                for (vector<int>& point : points) {
                    double x = point[0];
                    double y = point[1];
                    if (pow((x - x0), 2) + pow((y - y0), 2) <= rsqre) {
                        cnt++;
                    }
                }

                res = max(res, cnt);
            }
        }

        return res;
    }
};

///////////////////////////////////////////////////////////////////////////////

class Solution : public SolutionEnumeratePairs { };
// class Solution : public SolutionFindLastOf { };

///////////////////////////////////////////////////////////////////////////////

vector<pair<pair<vector<vector<int> >, int>, int> > tpairs {
    { { { { -2, 0 }, { 2, 0 }, { 0, 2 }, { 0, -2 } }, 2 },
        4 },
    { { { { -3, 0 }, { 3, 0 }, { 2, 6 }, { 5, 4 }, { 0, 9 }, { 7, 8 } }, 5 },
        5 },
};


// ß Expected: TODOC.
TEST(SolutionEnumeratePairs, numPoints)
{
    SolutionEnumeratePairs soln;

    for (auto& tpair : tpairs) {
        auto points = tpair.first.first;
        auto radius = tpair.first.second;
        auto actual = soln.numPoints(points, radius);
        EXPECT_EQ(actual, tpair.second);
    }
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
