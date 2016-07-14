// ArrayAlgo.cpp
// Author: Sprax Lines
// If this looks Javanese, it's because I translated some of it from my 
// ArrayAlgo.java

#include "stdafx.h"
#include "ArrayAlgo.hpp"

int ArrayAlgo::maxContiguousSum(int A[], unt len)
{
    int maxSum = 0;
    int maxNow = 0;
    for (unt j = 0; j < len; j++) {
        maxNow = max(0, maxNow + A[j]);
        maxSum = max(maxNow, maxSum);
    }
    return maxSum;
}

int ArrayAlgo::maxPositiveContiguousProduct(int A[], unt len)
{
    int maxPro = 0;
    int maxNow = 1;
    for (unt j = 0; j < len; j++) {
        maxNow = max(1, maxNow * A[j]);
        maxPro = max(maxNow, maxPro);
    }
    return maxPro;
}



pair<int, int> ArrayAlgo::indicesContainsTwoEntriesThatSumToN(int arr[], size_t size, int sum)
{
    hash_map<int, int> val2idx;
    for (size_t j = 1; j < size; j++) {
        int needed = sum - arr[j];
        hash_map<int, int>::const_iterator found = val2idx.find(needed);
        if (found != val2idx.end()) {
            return pair<int, int>(arr[j], j);
        }
        val2idx.insert(pair<int, int>(arr[j], j));
    }
    return pair<int, int>(-1, -1);
}

bool ArrayAlgo::arrayContainsTwoEntriesThatSumToN(int *pint, int len, int sum)
{
    hash_set<int> seen;
    while (--len >= 0) {
        if (seen.count(sum - *pint) > 0) {
            return true;
        }
        seen.insert(*pint++);
    }
    return false;
}

bool ArrayAlgo::test_arrayContainsTwoEntriesThatSumToN()
{
    int arr[] = { 1, 2, 3, 4, 5, 5, 7 };// N = 7
    bool does = arrayContainsTwoEntriesThatSumToN(arr, 7, 10);
    cout << "arrayContainsTwoEntriesThatSumToN got " << (does ? "true" : "false") << endl;
    return does;
}


#if 0

static int * maxMinContiguousProduct(int iA[], unt len) // TODO: seems to work -- test it more!
{
    int maxProduct = 0, maxPositive = 0; 
    int minProduct = 0, minNegative = 0;
    int minTemp;
    for ( int ia : iA ) {
        if (ia > 0) {
            maxPositive = max(ia, ia * maxPositive);
            if (minNegative < 0)
                minNegative = ia * minNegative;
        } else if (ia < 0) {
            minTemp = min(ia, ia * maxPositive);
            if (maxPositive > 0)
                maxPositive = ia * minNegative;
            minNegative = minTemp;
        } else {                  // ia == 0
            maxPositive = 0;
            minNegative = 0;
            continue;
        }
        maxProduct  = max(maxPositive, maxProduct);
        minProduct  = min(minNegative, minProduct);
    }
    int ret[] = { maxProduct, minProduct };
    return ret;
}  


static double * maxAndMinContiguousProduct(double DD[]) // TODO: does not work?
{
    double maxProduct  = 0.0, minProduct  = 0.0; 
    double maxPositive = 0.0, minNegative = 0.0;
    double maxTemp;
    for ( double dd : DD ) {
        if (dd > 0) {
            if (maxPositive > 0)
                maxPositive = max( 1, maxPositive * dd);
            minNegative = min(-1, minNegative * dd);
        } else if (dd < 0) {
            maxTemp     = max( 1, minNegative * dd);
            minNegative = min(-1, maxPositive * dd);
            maxPositive = maxTemp;
        } else { // dd == 0
            maxPositive =  0;
            minNegative =  0;
            continue;
        }
        maxProduct  = max(maxPositive, maxProduct);
        minProduct  = min(minNegative, minProduct);
    }
    double ret[] = { maxProduct, minProduct };
    return ret;
}
#endif



//////////////////////////////////////// PAIRS //////////////////////////////////////////
ArrayAlgo::IntPair ArrayAlgo::firstPairSumToN(int arr[], unt len, int sum)
{
    IntPair pair(-1, -1);  // return values: invalid indices indicate failure
    std::map<int, int> val2idx;
    for (unt j = 0; j < len; j++) {
        // If there is a previously stored index i s.t. A[i] + A[j] == sum, return <i,j>
        if (val2idx.find(sum - arr[j]) != val2idx.end()) {
            pair.first = val2idx[sum - arr[j]];
            pair.second = j;
            break;
        }
        // Store only the first occurrence of each array value
        if (val2idx.find(arr[j]) == val2idx.end())
            val2idx.insert(std::map<int, int>::value_type(arr[j], j));
    }
    return pair;
}

int ArrayAlgo::test_pairs()
{
    //int iB[] = { 1, 2, 0, -2, 3, -4, 0 };
    int iB[] = { -1, 2, 1, 2, -3, 0, 4 };
    unt len = sizeof(iB) / sizeof(int);
    for (int sum = 0; sum < 10; sum += 3) {
        puts("\n");
        IntPair pair = firstPairSumToN(iB, len, sum);
        printArray(iB, len);
        printf(" => 1st pair to add up to %d: [%d, %d]\n", sum, pair.first, pair.second);
    }
    return 0;
}


/////////////////////////////////// CONTIGUOUS SUBARRAYS ///////////////////////////////////////////
/**
 * Naive algorithm: time = O(N^2), additional space = O(1)
 * @param arr Input array of pos and neg int's.
 * @param sum The number to which which the sub-array is to add up to.
 * @return as references: first and last index of the first sub-array adding
 * up to sum, where the first sub-array is the one that begins first,
 * not necessarily the one that ends first.  Thus in [2, 1, 0, -1, 2],
 * the first sub-array summing to 0 is [1, 0, -1] with first and last
 * indices <1,3>, and not [0] with indices <2, 2>.
 * @return value: 0 on success, -1 if no such subarray was found.
 */
int ArrayAlgo::firstContiguousSumToN_naiveN2(const int arr[], unt len, int sum, int &rFirst, int &rLast)
{
    rFirst = rLast = -1;
    for (unt j = 0; j < len; j++) {
        int partial = 0;
        for (unt k = j; k < len; k++) {
            partial += arr[k];
            if (partial == sum) {
                rFirst = j;
                rLast = k;
                return 0;
            }
        }
    }
    return -1;
}

int ArrayAlgo::test_contiguousSubArrays()
{
    //int iB[] = { 1, 2, 0, -2, 3, -4, 0 };
    int iB[] = { -1, 2, 1, 2, -3, 0, 4 };
    unt len = sizeof(iB) / sizeof(int);
    int kFirst, kLast;
    for (int sum = 0; sum < 10; sum += 3) {
        puts("\n");
        int err = firstContiguousSumToN_naiveN2(iB, len, sum, kFirst, kLast);

        printArray(iB, len);
        printf(" => 1st contiguous sum to %d: [%d, %d] (naive algo)", sum, kFirst, kLast);
        /*
        int range[] = firstContiguousSumToN(iB, sum);
        printArray(iB, len);
        " => 1st contiguous sum to " + sum + ": [" + range[0] + ", " + range[1] + "]");
        */
    }

    ////int stat = test_maxContiguousSubArray();

    return 0;
}



int ArrayAlgo::unit_test(int level)
{
    puts("ArrayAlgo.unit_test\n");
    int stat = 0;

    stat += test_pairs();
    stat += test_contiguousSubArrays();

    if (level > 1) {

        //stat += test_triplets();
        //stat += test_fullHouse();
        //stat += test_interleave();
        //stat += test_nonRandomShuffle();
        //stat += test_circleStart(5,1);
    }
    return stat;
}





