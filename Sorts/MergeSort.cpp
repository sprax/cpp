// MergeSort.cpp : Sprax 2015.07
#include "stdafx.h"

#include "MergeSort.h"

template <typename T>
inline static void swap(T array[], int j, int k)
{
	T   temp = array[j];
	array[j] = array[k];
	array[k] = temp;
}

// @Deprecated
template <typename T>
static void mergeArrayLeftRightMemcpy(T array[], T temp[], unsigned int lo, unsigned int mid, unsigned int hi)
{
    unsigned int size = (hi - lo + 1);
    if (size < 3)
    {
        if (size == 2)
        {
            if (array[lo] > array[hi])
            {
                swap(array, lo, hi);
            }
        }
        return;
    }
    
    // Copy left/right-sorted array content into temp; the merge will copy it back into array totally-sorted.
    memcpy(temp + lo, array + lo, (hi - lo + 1)*sizeof(T));

    unsigned int arrayIdx = lo, tempLeft = lo, tempRight = mid + 1;
    while (arrayIdx <= hi)
    {
        if (temp[tempLeft] <= temp[tempRight])
        {
            array[arrayIdx++] = temp[tempLeft++];
            if (tempLeft > mid)
            {
                // The left side is consumed; copy the rest from the right side and terminate the loop.
                ////memcpy(array + arrayIdx, temp + tempRight, (hi - tempRight + 1)*sizeof(T));
                while (arrayIdx <= hi)
                    array[arrayIdx++] = temp[tempRight++];
                break;
            }
        }
        else
        {
            array[arrayIdx++] = temp[tempRight++];
            if (tempRight > hi)
            {
                // The right side is consumed; copy the rest from the left side and terminate the loop.
                ////memcpy(array + arrayIdx, temp + tempLeft, (mid - tempLeft + 1)*sizeof(T));
                while (arrayIdx <= hi)
                    array[arrayIdx++] = temp[tempLeft++];
                break;
            }
        }
    }
    return;
}



template <typename T>
void mergeArrayLeftRight(T array[], T temp[], unsigned int lo, unsigned int mid, unsigned int hi)
{
    unsigned int aI = lo, tempLeft = lo, tempRight = mid + 1;

    // Copy left/right-sorted array content into temp; the merge will copy it back into array totally-sorted.
    for (aI = lo; aI <= hi; aI++)
        temp[aI] = array[aI];

    for (aI = lo; aI <= hi; aI++)
    {
        if (temp[tempLeft] <= temp[tempRight])
        {
            array[aI] = temp[tempLeft++];
            if (tempLeft > mid)
            {
                // The left side is consumed; copy the rest from the right side and terminate the loop.
                while (++aI <= hi)
                    array[aI] = temp[tempRight++];
                break;
            }
        }
        else
        {
            array[aI] = temp[tempRight++];
            if (tempRight > hi)
            {
                // The right side is consumed; copy the rest from the left side and terminate the loop.
                while (++aI <= hi)
                    array[aI] = temp[tempLeft++];
                break;
            }
        }
    }
}

template <typename T>
static void mergeSortHelper(T array[], T extra[], unsigned int lo, unsigned int hi)
{
    if (lo >= hi)
        return;

    unsigned int mid = (lo >> 1) + (hi >> 1);
    mergeSortHelper(array, extra, lo, mid);
    mergeSortHelper(array, extra, mid + 1, hi);
    mergeArrayLeftRight(array, extra, lo, mid, hi);
}

template <typename T>
void mergeSort(T array[], unsigned int length)
{
	if (array == NULL || length < 2)
		return;

    T *extra = new T[length];
    mergeSortHelper(array, extra, 0, length - 1);
    delete [] extra;
}

template <typename T>
int test_mergeSortArray(T array[], unsigned int length)
{
	puts("test_mergeSort:");
	printf("input array: ");
	putsArray(array, length);
	mergeSort(array, length);
	printf("mergeSorted: ");
	putsArray(array, length);
	return 0;
}

/** TODO: test a long random array using median of 3 pivot */
int test_mergeSort(int level)
{
	int err = 0;

    if (level > 1)
    {
        int array[] = { 0, -1, 2, -3, 4, -5, 6, -7, 8, -9 };
        unsigned int length = sizeof(array) / 4;
        err += test_mergeSortArray(array, length);

        swap(array, 0, length - 1);
        swap(array, 1, length / 2);
        swap(array, 2, length / 3);
        swap(array, 3, length * 2 / 3);
        err += test_mergeSortArray(array, length);

        int arrB[] = { 0, -1, 11, -2, 12, -3, 13, -4, 14, -5, 15, -6, 16, -7, 17, -8, 18, -9, 19, -10, 20 };
        int lenB = sizeof(arrB) / 4;
        err += test_mergeSortArray(arrB, lenB);
        randomShuffle(arrB, lenB, 0);
        err += test_mergeSortArray(arrB, lenB);
    }

    std::string arrS[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten" };
    int lenS = 9;
    err += test_mergeSortArray(arrS, lenS);
    randomShuffle(arrS, lenS, 0);
    err += test_mergeSortArray(arrS, lenS);

    printf("test_mergeSort returning %d : %s\n", err, (err == 0 ? "PASS" : "FAIL"));
    return err;
}



