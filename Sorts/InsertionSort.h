// QuickSort.h

#pragma once

#include "SortInterfaces.h"


template <typename T>
class InsertionSort : public ArraySort<T>
{
public:
    static void insertionSort(T a[], size_t size);
    static void insertionSortSwap(T a[], size_t size);

    virtual void sort(T array[], size_t size)
    {
        insertionSort(array, size);
    }
    virtual void sortSwap(T array[], size_t size)
    {
        insertionSortSwap(array, size);
    }
};
