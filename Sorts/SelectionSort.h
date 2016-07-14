// QuickSort.h

#pragma once

#include "SortInterfaces.h"

template <typename T>
class SelectionSort : public ArraySort<T>
{
protected:
    template <typename T>
    inline static void swap(T array[], int j, int k)
    {
        T   temp = array[j];
        array[j] = array[k];
        array[k] = temp;
    }

public:
    // from Sedgewick
    template <typename T>
    static void selectionSort(T tA[], size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            size_t min = i;
            for (size_t j = i + 1; j < size; j++)
            {
                if ((tA[min] > tA[j]))
                    min = j;
            }
            swap(tA, i, min);
            assert(ArraySortUtil<T>::isSorted(tA, 0, i));
        }
        assert(ArraySortUtil<T>::isSorted(tA, size));
    }

    virtual void sort(T array[], size_t size)
    {
        selectionSort(array, size);
    }
};
