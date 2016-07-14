// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "BinLink.hpp"
#include "BinTree.hpp"
#include "BoggleBoard.hpp"
#include "Sum123.hpp"
#include "RandModN.hpp"
#include "slicingProblem.h"
#include "tests.h"

/** Kadane in Python (wikipedia)
def max_subarray(A):
max_so_far = max_ending_here = 0
for x in A:
max_ending_here = max(0, max_ending_here + x)
max_so_far = max(max_so_far, max_ending_here)
return max_so_far
*/
static int max(int a, int b) { return a > b ? a : b; }

int max_subarray(int A[], int countA)
{
    // This version of the algorithm assumes that at least one element is >= 0, OR, 
    // that we can take a zero-length sub-array with sum 0 as the max sub-array.  
    // Either way, we don't need to keep any subarray (even of length 1) whose sum is < 0.
    int max_so_far = 0;
    int max_ending_here = 0;
    for (int j = 0; j < countA; j++) {
        // If the running sum up to and including this element is < 0, drop it and take 0 instead; 
        // it could only diminish the sum we're after.
        max_ending_here = max(0, max_ending_here + A[j]);
        max_so_far = max(max_so_far, max_ending_here);
    }
    return max_so_far;
}

int Kadane(int* A, int n, int* pk, int* pl)
{
    *pk=0;
    *pl=0;
    int s=1<<31;    // INT_MIN
    int t=0;
    int j=0;
    for(int i=0; i < n ; i++)
    {
        t=t+A[i];
        if(t>s)
        {
            *pk=j;
            *pl=i;
            s=t;
        }
        if(t<0)
        {
            t=0;
            j=i+1;
        }
    }
    return s;
}

int test_Kadane(void)
{
    int A[] = { -2, 1, -3, 4, -1, 2, 1, -5, 4 };
    int countA = sizeof(A)/sizeof(int);

    int maxSum = max_subarray(A, countA);

    int beg, end;
    int maxContSum = Kadane(A, countA, &beg, &end);
    if (maxContSum == 6 && beg == 3 && end == 6) 
        return 0;
    return -1;
}

int main(int argc, char* argv[])
{
    int status = 0;
    bool testAll = false;
    if  (testAll) {
        status += test_rotatedString();
        status += test_threeWayPartition();
        status += test_slicingProblem();
        status += test_charString();
        status += test_fibmemo();
        status += test_for_each();
        status += BoggleBoard::unit_test();
        status += Sum123::unit_test();
        status += RandModN::unit_test();
        status += BinLink::unit_test();
        status += test_NlightBulbs(100);
        status += test_Kadane();
        status += test_interleaveInPlace();

    }
    status += test_uniqueCharSubString();
    status += test_macro();
    status += test_bitvector();
    //getchar(); // modify commnad window properties while waiting for input
    return status;
}

