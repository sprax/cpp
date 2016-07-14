// MergeSort.h

#pragma once

#include "SortInterfaces.h"

/* SinLink: singly-linked list node */
struct SinLink
{
    int data;
    struct SinLink * next;

    SinLink(int data, SinLink * next)
    {
        this->data = data;
        this->next = next;
    }
};

size_t listSize(SinLink *list);

int test_mergeSortSinLink(int level);


/* function prototypes */
SinLink* SortedMerge(SinLink* a, SinLink* b);

// Provides methods to sort a singly-linked list by changing next pointers (not data).
class MergeSortSinLink : public ListSort<SinLink>
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

