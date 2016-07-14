// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "maxSubArrays.h"
#include "printArray.h"

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

int Kadane(int arr[], int arrLen, int* pFirst, int* pLast)
{
    *pFirst = 0;
    *pLast = 0;
    int maxSum = 1 << 31;    // INT_MIN
    int tmpSum = 0;
    for(int tmpFirst = 0, tmpLast = 0; tmpLast < arrLen ; tmpLast++) 
    {
        tmpSum += arr[tmpLast];
        if (maxSum < tmpSum) {
            maxSum = tmpSum;
            *pFirst = tmpFirst;
            *pLast  = tmpLast;
        }
        if (tmpSum < 0) {
            tmpSum = 0;
            tmpFirst = tmpLast + 1;
        }
    }
    return maxSum;
}

int test_Kadane(void)
{
    int A[] = { -2, 1, -3, 4, -1, 2, 1, -5, 4 };
    int countA = sizeof(A)/sizeof(int);

    int maxSum = max_subarray(A, countA);

    int first, last;
    int maxContSum = Kadane(A, countA, &first, &last);
    int stat =  -1;
    if (maxContSum == 6 && first == 3 && last == 6) 
        stat = 0;

    putsArray(A, countA, " gives max subArray:");
    printSubArray(A, first, last, true);
    printf(" <start, last, sum> = <%2d  %2d  %2d>\n", first, last, maxContSum);  
    return stat;
}


  /** TODO
  Amazon Interview Question for Software Engineer / Developers about Coding
  aniketnit on October 24, 2011

  Suppose there is a circle. You have five points on that circle. Each
  point corresponds to a petrol pump. You are given two sets of data.

  1. The amount of petrol that petrol pump will give.
  2. Distance from that petrol pump tp the next petrol pump.

  (Assume for 1 lit Petrol the truck will go 1 km)

  Now calculate the first point from where a truck will be able to
  complete the circle.
  (The truck will stop at each petrol pump and it has infinite
  capacity).
  Give o(n) solution. You may use o(n) extra space.
  
  * IDEA: for each point on the circle, calculate how mucy excess gas
  * you'd have if you had started at this point and made it all the
  * way arond and back to it, stopping at each of the other points.
  * If the running excess at point j is < 0, set first = j+1, and if
  * first > len, that means it cannot be done.  
  *
  */
  int circleStartOlder(const double gas[], const double dst[], int length, double *pExtraGas)
  {
    *pExtraGas = 0.0;
    int first = -1;
    double *excess = new double[length];   // TODO: delete []
    double  excessTmp = 0.0;
    for(int len2 = length*2, tmpFirst = 0, tmpLast = 0; tmpLast < len2 ; tmpLast++) 
    {
        excess[tmpLast % length] = gas[tmpLast % length] - dst[tmpLast % length];
        excessTmp += excess[tmpLast % length];

        if (tmpLast - tmpFirst >= length - 1) {
            *pExtraGas = excessTmp;
            first = tmpFirst;
            break;
        }
        if (excessTmp < 0.0) {
            excessTmp = 0.0;
            tmpFirst = tmpLast + 1;
        }
    }
    delete [] excess;
    return first;
  }    
    
  
  int circleStartOld(const double gas[], const double dst[], int length, double *pExtraGas)
  {
    *pExtraGas = 0.0;
    int first = -1, tmpFirst = 0, tmpLast = 0;
    double  excessTmp = 0.0, excessTot = 0.0, excessPnt;
    for( ; tmpLast < length ; tmpLast++) 
    {
        excessPnt  = gas[tmpLast] - dst[tmpLast];
        excessTmp  += excessPnt;
        excessTot += excessPnt;  // update this only in 1st pass (we could update this only when setting excessTmp = 0, but don't...)
        if (excessTmp < 0.0) {
            excessTmp = 0.0;
            tmpFirst = tmpLast + 1;
        }
    }
    // Trivial cases:
    if (tmpFirst == 0) {            // Starting at point 0 just works
        first = tmpFirst;           
        *pExtraGas = excessTmp;
    } else if (excessTot < 0.0) {   // Starting at any point fails, 
        *pExtraGas = excessTot;     // so return start of -1 & total excess < 0.
    } else {        // General case: solution exists, but first >= tmpFirst > 0
        // Cycle through the array in 2nd pass, continuing where we left off, 
        // with tmpLast == array length.  If tmpLast - tmpFirst == length - 1, 
        // then tmpFirst is the first index of the desired "wrap-around subarray."
        // Even in the worst case 2nd pass, we only loop to length - 1, since
        // we've already checked first == 0.
        for (int lenM1 = length - 1, j = 0; j < lenM1 ; j++, tmpLast++) 
        {
        excessPnt = gas[j] - dst[j];
        excessTmp += excessPnt;
            if (tmpLast - tmpFirst == lenM1) {
                *pExtraGas = excessTmp;
                first = tmpFirst;
                break;
            }
            if (excessTmp < 0.0) {
                excessTmp = 0.0;
                assert("this can't happen" == "yes it can");
                tmpFirst = tmpLast + 1;
            }
        }
        assert(first > 0); // excess gas >= 0 means there must be a solution.
        assert(first < length);
        assert(abs(*pExtraGas - excessTot) < 0.001); // excess gas is invariant
    }    
    return first;
  }
  
  
  int circleStart(const double gas[], const double dst[], int length, double *pExtraGas)
  {
    // The gas left over after driving the whole cirlce is an invariant.
    // As the total diff between gas*mpg and miles, it's teh same no matter
    // where you start adding -- becaues addition is commutative.  If this 
    // quantity < 0.0, there is no solution.  If it's > 0, there is a solution.
    *pExtraGas = 0.0;   // This is a circle invariant.  
    int tmpFirst = 0, tmpLast = 0;
    double  excessTmp = 0.0, excessPnt;
    for( ; tmpLast < length ; tmpLast++) 
    {
        excessPnt  = gas[tmpLast] - dst[tmpLast];
        excessTmp  += excessPnt;
        *pExtraGas += excessPnt; // (we could update this only when setting excessTmp = 0, but don't...)
        if (excessTmp < 0.0) {
            excessTmp = 0.0;
            tmpFirst = tmpLast + 1;
        }
    }
    if (*pExtraGas < 0.0) {     // Starting at any point fails, 
        return -1;              // So return invalid index.
    }
    return tmpFirst;           
  }
  

  int test_circleStart(int numPoints, int numTrials)
  {
    double gasTot = 0.0, *gas = new double[numPoints];
    double dstTot = 0.0, *dst = new double[numPoints];

    for (int j = 0; j < numPoints; j++) {
      gas[j] =      1.0 + cos((double)j);
      dst[j] = 1.0;
      gasTot += gas[j];
      dstTot += dst[j];
    }
    gas[3] += gas[0];
    gas[2] -= gas[0];
    if (dstTot > gasTot) {
      double dif = dstTot - gasTot + 0.01;
      gas[numPoints-1] += dif;
      gasTot += dif;
    }
    double extraGas = 99999999.9;
    int start = circleStart(gas, dst, numPoints, &extraGas);
    //putsArray("circleStart gas:", gas, " ("+gasTot+")");
    //putsArray("       distance:", dst, " ("+dstTot+") got start = " + start);
    
    double G[] = {  6,  2,  5,  3,  5,  8 }; // 29
    double D[] = {  1,  6,  1,  9,  4,  6 }; // 27 total
    double F[] = {  5,  1,  5, -1,  0,  2 };      // excess starting at index 0
    double H[] = {          0, -1,  0,  3, -1 };  //        starting at index 2

    int len = sizeof(G)/sizeof(double);
    start = circleStart(G, D, len, &extraGas);
    printf("circleStart got start(%d)  extraGas(%g)\n", start, extraGas);
    //putsArray("circleStart gas:", G, len);
    //putsArray("       distance:", D);
    return start;
  }
