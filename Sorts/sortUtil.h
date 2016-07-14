// MergeSortSinLinkRec.h

#pragma once

template<typename T>
class ArraySortUtil
{
public:
    static bool isSorted(T A[], size_t size)
    {
        return isSorted(A, 0, size-1);
    }

    static bool isSorted(T A[], size_t lo, size_t hi)
    {
        while (hi > lo)
        {
            if (A[hi] < A[hi - 1])
                return false;
            hi--;
        }
        return true;
    }

};

