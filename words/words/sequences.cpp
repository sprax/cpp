// sequendew.cpp
// Author: Sprax Lines 2011 Oct 27

#include "stdafx.h"

#include "arrayUtil.h"
#include "tests.h"

/**
 * isArrayConsecutive(A, len) returns true iff the array A is 
 * non-empty and contains all the integers in some contiguous range
 * with no duplicates, not necessarily in order.  
 * 
 * A is const; it cannot be modified.
 * The algorithm here is O(N) time, O(N/4) space 
 * (assuming sizeof(bool) == sizeof(int)/4).
 */
bool isArrayConsecutiveConst(int A[], size_t len) // Do not modify A[]
{
    int min, max;
    if ( ! getMinAndMax<int>(A, len, min, max))
        return false;

    int dif  = max - min;
    if (dif != len - 1)
        return false;

    // The range is correct; return false only if there are duplicates
    bool *T = new bool[len];    // All true by default constructor
    //bool *T = (bool*)calloc(len, sizeof(bool));
    for (size_t j = 0; j < len; j++) {
        if ( ! T[A[j] - min]) {
            delete [] T;
            //free(T);
            return false;   // duplicate number
        }
        else {
            T[A[j] - min] = false; // check off this number
        }
    }
    delete [] T;
    //free(T);
    return true;
}

/**
 * isArrayConsecutive(A, len) returns true iff the array A is 
 * non-empty and contains all the integers in some contiguous range
 * with no duplicates, not necessarily in order.  
 * 
 * A is NOT const; it can be modified.
 * The algorithm here is O(1) space and supposedly O(N) time, even
 * though it looks like O(N^2) time.
 */
bool isArrayConsecutiveModSwap(int A[], size_t len) // can modify A[]
{
    int min, max;
    if ( ! getMinAndMax<int>(A, len, min, max))
        return false;

    int dif  = max - min;
    if (dif != len - 1)
        return false;
    // The range is correct; return false only if there are duplicates
    for (size_t k, j = 0; j < len; j++) {
        k = A[j] - min;     // index where value A[j] would be if A were consec and sorted
        while (j != k) {
            if (A[j] == A[k])
                return false;   // found duplicate
            swapArray(A, j, k);
            k = A[j] - min;
        }
    }
    return true;
}

/**
 * isArrayConsecutive(A, len) returns true iff the array A is 
 * non-empty and contains all the integers in some contiguous range
 * with no duplicates, not necessarily in order.  
 * 
 * A is NOT const; it can be modified.
 * The algorithm here is O(1) space and supposedly O(N) time, even
 * though it looks like O(N^2) time.
 */
bool isArrayConsecutiveMod_bust(int A[], size_t len) // can modify A[]
{
    int min, max;
    if ( ! getMinAndMax<int>(A, len, min, max))
        return false;

    int dif  = max - min;
    if (dif != len - 1)
        return false;
    // The range is correct; return false only if there are duplicates
    // Try sending A[j] forward to its correct place:
    int tVal = A[0], tIdx = -1;
    for (size_t k, j = 0; j < len;  ) {
        k = A[j] - min;     // index where value A[j] would be if A were consec and sorted
        if (k != j) {                   // if A[j] is not already in its sorted spot...
            if (A[j] == A[k])
                return false;           // found duplicate
            if (A[k] - min == j) {
                swapArray(A, j, k);     // A[j] and A[k] were in each other's sorted spots
                j++;                    // go to the next element
            }
            else if (tIdx == -1) {      // temp space is empty
                tIdx = j;               // remember the new vacated spot
                tVal = A[k];            // remember the value before stomping on it
                A[k] = A[j];            // move the value at j to its sorted spot
                j++;       
            } else if (A[j] == tVal) {  
                return false;
            } else {
                // swap smallest value into spot j
                swapArray(A, j, k);
                // j++;     // DON'T INCREMENT J !!!!

            }
        }
    }
    return true;
}


int test_isArrayConsecutive(boolFunc_intArray isConsecutive, int len, int offset) 
{
    int *A = new int[len];
    for (int j = 0; j < len; j++)
        A[j] = j + offset;
    randomShuffle(A, len, (unsigned int)time(NULL));
    bool bTrue  = isConsecutive(A, len);
    char *yesno = bTrue ? " fills a range" : " does not";
    putsArray(A, len, yesno);
    A[len/2] = A[len-1];  // duplicate entry 
    bool bDupe =  isConsecutive(A, len);
    yesno = bDupe ? " fills a range" : " has a duplicate";
    putsArray(A, len, yesno);
    A[len/2] = len + offset + 2;  // out of range 
    bool bOut = isConsecutive(A, len);
    yesno = bOut ? " fills a range" : " is missing a value";
    putsArray(A, len, yesno);
    delete [] A;
    if (bTrue && ! bDupe && ! bOut)
        return 0;
    return -1;
}
