// MergeSortSinLink.cpp : Adapted from GeeksForGeeks
#include "stdafx.h"

#include "InsertionSort.h"
#include "MergeSortSinLinkRec.h"
#include "MergeSortSinLinkTest.h"
#include "QuickSort.h"
#include "SelectionSort.h"
#include "SortTests.h"
#include "SortUtil.h"


template <typename T>
inline static void swap(T A[], int j, int k)
{
    T tv = A[j];
    A[j] = A[k];
    A[k] = tv;
}



template <typename T>
void insertionSort(T A[], size_t size);





template <typename T>
void insertionSortLazy(T A[], size_t size)
{
    for (size_t j = 1; j < size; j++)
    {
        T valj = A[j];                  // Get the value at index j
        for (size_t k = j; k > 0;)
        {
            if (valj < A[k - 1])
            {
                A[k] = A[k - 1];
                if (--k == 0) {
                    A[k] = valj;
                    break;
                }
            }
            else
            {
                if (k != j)
                {
                    A[k] = valj;
                }
                break;
            }
        }
    }
}

template <typename T>
void insertionSortDoWhile(T A[], size_t size)
{
    for (size_t j = 1; j < size; j++)
    {
        T valj = A[j];                  // Get the value at index j
        size_t k = j;
        do {
            if (valj < A[k - 1])
            {
                A[k] = A[k - 1];
                if (--k == 0) {
                    A[k] = valj;
                    break;
                }
            }
            else
            {
                if (k != j)
                {
                    A[k] = valj;
                }
                break;
            }
        } while (k > 0);
    }
}




template <typename T>
int test_ArraySortMethod(std::string label, void(*sortMethod)(T[], size_t), const T tA[], size_t size)
{
    printf("test_ArraySortMethod: %s : input: \n", label.c_str());
    T *A = new T[size];
    for (size_t j = 0; j < size; j++)
        A[j] = tA[j];
    putsArray(A, size);
    sortMethod(A, size);
    bool sorted = ArraySortUtil<T>::isSorted(A, size);
    printf("After sorting:  sorted=%d\n", sorted);
    putsArray(A, size);
    puts("");
    delete[] A;
    return -1 + sorted;
}


int test_arraySorts(int level)
{
    const int A[] = { 9, 8, 4, 7, 1, 3, 4, 3, 5, 2, 4, 6, 4, 2, 3 };
    size_t size = sizeof(A) / 4;

    //  InsertionSort<int> *is = new InsertionSort<int>();
    test_ArraySortMethod("insertionSortSwap   ", &(InsertionSort<int>::insertionSortSwap), A, size);
    test_ArraySortMethod("insertionSortMove   ", &(InsertionSort<int>::insertionSort), A, size);
    test_ArraySortMethod("insertionSortLazy   ", &(insertionSortLazy<int>), A, size);
    test_ArraySortMethod("insertionSortDoWhile", &(insertionSortDoWhile<int>), A, size);
    test_ArraySortMethod("SelectionSortSwap   ", &(SelectionSort<int>::selectionSort), A, size);
    test_ArraySortMethod("QuickSort defaults  ", &(QuickSort<int>::quickSort), A, size);

    // MS compiler (at least) doesn't like this attempt to reference the method via the object:
    // QuickSort<int> *qs = new QuickSort<int>();
    // test_ArraySortMethod("QuickSort instance  ", qs->quickSort, A, size);
    
    return 0;
}