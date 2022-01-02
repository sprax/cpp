#!/usr/bin/env python3
# @file: test_1453h_max_darts_in_any_circle.py
# @auth: Sprax Lines
# @date: 2020-05-17 23:31:59 Sun 17 May

import math
import statistics
from collections import defaultdict
from pdb import set_trace
from typing import List

from leet_test import MakeTests

'''
1453. Maximum Number of Darts Inside of a Circular Dartboard.  Hard

You have a very large square wall and a circular dartboard placed on the wall.
You have been challenged to throw darts into the board blindfolded.
Darts thrown at the wall are represented as an array of points on a 2D plane.

Return the maximum number of points that are within or lie on
ANY circular dartboard of radius r.

Example 1:
Input:  points = [[-2, 0], [2, 0], [0, 2], [0, -2]], r = 2
Output: 4
Logic:  Circle dartboard with center in (0, 0) and radius = 2 contain all
points.

Example 2:
Input:  points = [[-3, 0], [3, 0], [2, 6], [5, 4], [0, 9], [7, 8]], r = 5
Output: 5
Logic:  Circle dartboard with center in (0, 4) and radius = 5 contain all
points except the point (7, 8).

Example 3:
Input:  points = [[-2, 0], [2, 0], [0, 2], [0, -2]], r = 1
Output: 1

Example 4:
Input:  points = [[1, 2], [3, 5], [1, -1], [2, 3], [4, 1], [1, 3]], r = 2
Output: 4

Constraints:
1 <= points.length <= 100
points[i].length == 2
-10^4 <= points[i][0], points[i][1] <= 10^4
1 <= r <= 5000

Accepted 1775 / 6082 = 29.2%
'''


class SolutionAngSweepNew:
    '''
    [Python3] angular sweep O(N^2 logN)
    Based on code by ye15 (views 1.1K; votes 33; reputation 3384)
    Last Edit: May 17, 2020 12:26 AM

    Algo
    Pick a point, say P, from the set, and rotate a circle with fixed-radius r.
    During the rotation P lies on the circumference of the circle (note P is
    not the center) and maintain a count of the number of points in the circle
    at an angle Θ (between PC and x-axis, where C is the center of the circle).

    For every other point Q within 2*r distance to P, compute the angle Θ when
    Q enters the circle and the angle Θ when Q exits the circle with math.
    Sort the angles and scan through it to check the maximum points in the
    circle.

    Perform the above operation for all points to find the maximum.

    Implementation (time complexity O(N^2 logN) | space complexity O(N))

    The sweep solution leveraged Thales's theorem that states that if A, B,
    and C are distinct points on a circle where the line AC is a diameter,
    then the angle ∠ABC is a right angle.
    https://en.wikipedia.org/wiki/Thales's_theorem

    Runtime: 128 ms, faster than 100.00% in Python3.
    Memory Usage: 15 MB, less than 20.00% in Python3.
    '''

    def numPoints(self, points: List[List[int]], r: int) -> int:
        from math import acos, atan2, sqrt
        ans = 1
        for x, y in points:
            angles = []
            for x1, y1 in points:
                if ((x1 != x or y1 != y)
                        and (d := sqrt((x1 - x)**2 + (y1 - y)**2)) <= 2 * r):
                    angle = atan2(y1 - y, x1 - x)
                    delta = acos(d / (2 * r))
                    angles.append((angle - delta, +1))  # entry
                    angles.append((angle + delta, -1))  # exit
            angles.sort(key=lambda x: (x[0], -x[1]))
            val = 1
            for _, entry in angles:
                ans = max(ans, val := val + entry)
        return ans

###############################################################################


class SolutionAngSweep:
    '''
    Runtime: 140 ms, faster than 97.25% in Python3.
    Memory Usage: 15.2 MB, less than 100.00% in Python3.

    Runtime: 184 ms, faster than 88.24% in Python3.
    Memory Usage: 14.9 MB, less than 100.00% in Python3.
    '''

    def numPoints(self, A: List[List[int]], r: int) -> int:

        four_r_sqrd = 4 * (r ** 2)
        twice_r = 2 * r
        cands = defaultdict(list)
        n = len(A)
        for i in range(n):
            for j in range(i + 1, n):
                d = sqrd(A[i], A[j])
                if d <= four_r_sqrd:
                    d = math.sqrt(d)
                    g_A = math.atan2((A[i][1] - A[j][1]), (A[i][0] - A[j][0]))
                    g_B = math.acos(d / twice_r)
                    cands[i].append((g_A - g_B, True))
                    cands[i].append((g_A + g_B, False))
                    g_A = math.atan2((A[j][1] - A[i][1]), (A[j][0] - A[i][0]))
                    cands[j].append((g_A - g_B, True))
                    cands[j].append((g_A + g_B, False))
        ret = 1
        for cd in cands.values():
            t = sorted(cd, key=lambda a: (a[0], not a[1]))
            count = 1
            for _, b in t:
                count += (1 if b else -1)
                ret = max(ret, count)
        return ret

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
        # print("[%d, %d] => dist %d <=? %d" % (x, y, dist, rsqr))
        if dist <= rsqr:
            count += 1
    return count

###############################################################################


def num_points_from(origin: List[int], points: List[List[int]], r: int) -> int:
    count = 0
    rsqr = r * r
    for point in points:
        dsqr = sqrd(origin, point)
        # print("[%d, %d] => dist %d <=? %d" % (x, y, dist, rsqr))
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
                numc = num_points_from([x, y], points, r)
                if maxc < numc:
                    maxc = numc
        return maxc


class SolutionSmall:    # FIXME
    def numPoints(self, points: List[List[int]], r: int) -> int:
        size = len(points)
        # pair_dists = [[None] * size for _ in range(size)]
        ans = 0
        for i in range(size - 1):
            for j in range(i + 1, size):
                pair_dist = sqrd(points[i], points[j])
                # pair_dists[i][j] = pair_dists[j][i] = sqrd(points[i],
                # points[j]);
                if pair_dist <= r * r * 2:
                    ans += 1
        return ans

###############################################################################


SOLNS = [
    SolutionAngSweep,
    # SolutionSmall,
    # SolutionOther,
    SolutionAngSweepNew,
]


class Solution(SOLNS[-1]):
    pass

###############################################################################


class TestSolutions:
    """ unit tests for Solution class """

    def test_solutions(self, verbose=2):
        """ unit tests for named solution classes """
        mtests = MakeTests(SOLNS, "numPoints")

        points_a = [[-2, 0], [2, 0], [0, 2], [0, -2]]
        radius_a = 2
        sample_a = [points_a, radius_a]
        expect_a = 4

        points_b = [[-3, 0], [3, 0], [2, 6], [5, 4], [0, 9], [7, 8]]
        radius_b = 5
        sample_b = [points_b, radius_b]
        expect_b = 5

        points_c = [[1, 2], [3, 5], [1, -1], [2, 3], [4, 1], [1, 3]]
        radius_c = 2
        sample_c = [points_c, radius_c]
        expect_c = 4

        tpairs = [
            (sample_a,    expect_a),
            (sample_b,    expect_b),
            (sample_c,    expect_c),
        ]
        mtests.test_all(tpairs, asserts=0)


if __name__ == "__main__":
    TestSolutions().test_solutions()
