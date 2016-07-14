// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "sortUtil.h"
#include "InsertionSort.h"

template <typename T>
inline static void swap(T array[], int j, int k)
{
	T   temp = array[j];
	array[j] = array[k];
	array[k] = temp;
}


template <typename T>
void InsertionSort<T>::insertionSort(T A[], size_t size)
{
    for (size_t j = 1; j < size; j++)
    {
        T valj = A[j];                  // Get the value at index j
        size_t k = j - 1;                          // Assuming the array is already sorted up to j-1,
        if (valj < A[k])
        {
            A[j] = A[k];
            for (; k > 0; k--)                    // find the position where this value belongs
            {
                if (valj < A[k - 1])
                    A[k] = A[k - 1];    // shift previously sorted values up, back-filling postion j, j-1, j-2, ...
                else
                    break;
            }
            A[k] = valj;
        }
    }
}


// from Sedgewick
template <typename T>
void InsertionSort<T>::insertionSortSwap(T a[], size_t size)
{
    for (size_t i = 0; i < size; i++) {
        for (size_t j = i; j > 0 && (a[j] < a[j - 1]); j--)
        {
            swap(a, j, j - 1);
        }
        assert(ArraySortUtil<T>::isSorted(a, 0, i));
    }
    assert(ArraySortUtil<T>::isSorted(a, size));
}

// This is here only to "trick" the compiler into generating the
// code for InsertionSort<int>::insertionSort(...), etc., so the
// linker can find it by its named address, as in passing the 
// following argument into a function: &(InsertionSort<int>::insertionSort)
// Without it, the linker gets a fatal error: unresolved external...
InsertionSort<int> *createInsertionSortInt()
{
    auto foo = new InsertionSort<int>();
    return foo;
}