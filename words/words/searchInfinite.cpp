// numberical.cpp

#include "stdafx.h"
#include "tests.h"


/////////////////////////////////////////////////////////////////////////////
/*
You are given an infinitely long array of integers in which some part of the 
array is sorted and all other entries contain terminating character ($). 
Given a number how will you search in the given array.  What is the complexity
of the search operation if the number is not present in the array?
*/
//Ex: 1 2 3 4 5 6 7 8 9 $ $ $ $ $ $ $ $ $ $ $ ….........................
// infA[] - infinite array with '$' as a terminating character
bool binary_search_infinite_array(char *infA, char x) 
{
    int l = 0, r = 1;
    while(1) { // first phase: find the array bounds
        char a = infA[r - 1];
        // here we either found the valid range or hit the end of the data:
        // in any case we can reduce the search space to [l; r]
        if(a == '$' || x < a) {
            break;
        }
        if(x == a) // found the actual val
            return true;
        // otherwise we keep on moving the right boundary
        l = r - 1, r = r * 2;
    }
    printf("'x' can only lie between [%c; %c]\n", infA[l], infA[r]);

    // second phase: run an ordinary binary search with account for
    // the terminating symbol '$'
    while(1) {
        int mid = (l + r) / 2;
        char a = infA[mid];
        if(a == '$' || x < a) {
            r = mid;
        } else if(x > a) {
            l = mid + 1;
        } else // gotcha !!
            return true;
        if(l == r)
            return false; // not found
    }
}

