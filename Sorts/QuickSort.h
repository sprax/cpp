// QuickSort.h

#pragma once

#include "SortInterfaces.h"

template <typename T>
int pivotIndexMedianOfThree(T array[], int x, int y, int z);

template <typename T>
int pivotIndexMedianOfFirstMiddleLast(T A[], int lo, int hi);

template <typename T>
int pivotIndexMedianRandom3(T A[], int lo, int hi);

int test_quickSort(int level);

typedef int(*quickPartitionInt)(int A[], int lo, int hi); 

template <typename T>
static int quickPartitionSedgewick(T array[], int lo, int hi);

template <typename T>
void quickSort(T array[], size_t size
    , int(*quickPartition)(T A[], int lo, int hi) = quickPartitionSedgewick);

template <typename T>
class QuickSort : public ArraySort<T>
{
public:
    static void quickSort(T A[], size_t size);
    
    virtual void sort(T A[], size_t size)
    {
        quickSort(A, size);
    }

    // TODO: Move this to a test class?
    static bool isSorted(T array[], size_t size)
    {
        if (size < 2)
            return true;
        T oldVal = array[0];
        for (size_t j = 1; j < size; j++)
        {
            T newVal = array[j];
            if (oldVal > newVal)
                return false;
        }
        return true;
    }

};