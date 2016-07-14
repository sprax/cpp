// MergeSortSinLinkRec.h

#pragma once

#include "MergeSortSinLink.h"

// Provides methods to sort a singly-linked list by changing next pointers (not data).
class MergeSortSinLinkRec : public ListSort < SinLink >
{
public:
    static void mergeSort(SinLink **list);
    static void mergeSort(SinLink **list, size_t size);
    virtual void sort(SinLink **list)
    {
        mergeSort(list);
    }
    virtual void sort(SinLink **list, size_t size)
    {
        mergeSort(list, size);
    }
};
