// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "QuickSort.h"
//#include "arrayUtil.h"

template <typename T>
inline static void swap(T array[], int j, int k)
{
	T   temp = array[j];
	array[j] = array[k];
	array[k] = temp;
}

static int pivotIndexMiddle(int lo, int hi)
{
	return (lo >> 1) + (hi >> 1);
}

/** x, y, and z must be valid indices in array[]; no bounds checking here.
 *  If we knew x < y < z, we could swap the min and max values right here,
 *  but in general we do not.
 */
template <typename T>
int pivotIndexMedianOfThree(T array[], int x, int y, int z)
{
	if (array[x] <= array[y])
	{
		if (array[y] <= array[z])
			return y;				// because Ax <= Ay and Ay <= Az, so Ax <= Ay <= Az
		if (array[x] <= array[z])
			return z;				// because Ax <= Ay and Ax <= Az but Az < Ay, so Ax <= Az < Ay
		else
			return x;				// because Ax <= Ay but Az < Ay and Az < Ax, so Az < Ax <= Ay
	}
	else // A[x] > A[y]
	{
		if (array[x] <= array[z])
			return x;				// because Ay < Ax and Ax <= Az, so Ay < Ax <= Az
		if (array[y] <= array[z])
			return z;				// because Ay < Ax and Ay <= Az but Az < Ax, so Ay <= Az < Ax
		else
			return y;				// because Ay < Ax and Az < Ax but Az < Ay, so Az < Ay < Ax
	}
}

static int test_pivotIndexMedianOfThree()
{
	int numWrong = 0;
	int A[] = { -4, 0, 9 };
	if (1 != pivotIndexMedianOfThree(A, 0, 1, 2))
		numWrong++;
	if (1 != pivotIndexMedianOfThree(A, 2, 1, 0))
		numWrong++;
	if (1 != pivotIndexMedianOfThree(A, 2, 0, 1))
		numWrong++;
	int B[] = { -1, 0, -2, 6, 5, 5, -7 };
	if (0 != pivotIndexMedianOfThree(B, 0, 1, 2))
		numWrong++;
	if (0 != pivotIndexMedianOfThree(B, 2, 5, 0))
		numWrong++;
	if (2 != pivotIndexMedianOfThree(B, 6, 0, 2))
		numWrong++;
	if (2 != pivotIndexMedianOfThree(B, 0, 2, 6))
		numWrong++;
	return numWrong;
}

template <typename T>
int pivotIndexMedianOfFirstMiddleLast(T A[], int lo, int hi)
{
	if (hi - lo < 2)
		return lo;
	else
		return pivotIndexMedianOfThree(A, lo, (lo >> 1) + (hi >> 1), hi);
}

template <typename T>
int pivotIndexMedianRandom3(T A[], int lo, int hi)
{
	int size = hi - lo + 1;
	if (size < 3)
	{
		return lo;		// Calling rand() would cost more than a few swaps
	}
	else
	{
		int r1 = lo + rand() % size;
		int r2 = lo + rand() % size;
		int r3 = lo + rand() % size;
		return pivotIndexMedianOfThree(A, r1, r2, r3);
	}
}

/** Adapted from Sedgewick */
template <typename T>
static int quickPartitionSedgewick(T array[], int lo, int hi)
{
	int beg = lo;
	int end = hi + 1;
	int pivotIndex = pivotIndexMiddle(lo, hi);
	T pivotValue = array[pivotIndex];
	swap(array, lo, pivotIndex);		// Store the chosen pivot value at [lo]
	while (true)
	{
		while (array[++beg] < pivotValue && beg < hi);
		while (array[--end] > pivotValue && end > lo);
		if (beg >= end)
			break;
		swap(array, beg, end);
	}
	swap(array, lo, end);				// Put the pivot value in its final place
	return end;
}

/** Adapted from Wikipedia
 *  lo is the index of the leftmost element of the subarray
 *  hi is the index of the rightmost element of the subarray (inclusive)
 */

template <typename T>
static int quickPartitionWikipedia(T array[], int lo, int hi)
{
	int pivotIndex = pivotIndexMiddle(lo, hi);
	T   pivotValue = array[pivotIndex];
	swap(array, pivotIndex, hi);		// Put the chosen pivot at [hi]
	int storeIndex = lo;
	// Compare remaining array elements against pivotValue = array[hi]
	for (int i = lo; i < hi; i++)
	{
		if (array[i] < pivotValue)
		{
			swap(array, i, storeIndex);
			storeIndex++;
		}
	}
	swap(array, storeIndex, hi);		// Move pivot to its final place
	return storeIndex;
}



template <typename T>
static void quickSortRecursive(T array[], int lo, int hi, int(*quickPartition)(T[], int, int))
{
    if (lo >= hi)
        return;

    int partIndex = (*quickPartition)(array, lo, hi);
    quickSortRecursive(array, lo, partIndex - 1, quickPartition);		// sort left partition
    quickSortRecursive(array, partIndex + 1, hi, quickPartition);		// sort right partition
}

template <typename T>
void quickSort(T array[], size_t size, int(*quickPartition)(T A[], int lo, int hi))
{
    if (array == NULL || size < 2)
        return;

    quickSortRecursive(array, 0, size - 1, quickPartition);
}

template <typename T>
void QuickSort<T>::quickSort(T array[], size_t size)
{
    if (array == NULL || size < 2)
        return;

    quickSortRecursive(array, 0, size - 1, quickPartitionSedgewick);
}

template <typename T>
int test_quickSortArray(T array[], size_t size)
{
	puts("test_quickSort:");
	printf("input array: ");
	putsArray(array, size);
	quickSort(array, size);
    bool sorted = QuickSort<T>::isSorted(array, size);
	printf("quickSorted:  sorted=%d\n", sorted);
	putsArray(array, size);
	return 0;
}

int test_quickSortRand(size_t size)
{
    int *A = new int[size];
    for (size_t j = 0; j < size; j++)
    {
        A[j] = rand();
    }
    test_quickSortArray(A, size);
    delete[] A;
    return 0;
}

// This is here only to "trick" the compiler into generating the
// code for SelectionSortSort<int>::selectionSort(...), etc., so the
// linker can find it by its named address, as in passing the
// following argument into a function: &(InsertionSort<int>::selectionSort)
// Without it, the linker gets a fatal error: unresolved external...
QuickSort<int> *createQuikSortInt()
{
#if 1
    auto inst = new QuickSort<int>();
    assert(inst);
    return inst;
#else
    return NULL;
#endif
}


/** TODO: test a long random array using median of 3 pivot */
int test_quickSort(int level)
{
	int err = 0;
	int array[] = { 0, -1, 2, -3, 4, -5, 6, -7, 8, -9 };
	int length = sizeof(array) / 4;
	err += test_pivotIndexMedianOfThree();
	err += test_quickSortArray(array, length);

	swap(array, 0, length - 1);
	swap(array, 1, length / 2);
	swap(array, 2, length / 3);
	swap(array, 3, length * 2 / 3);
	err += test_quickSortArray(array, length);

    int arrB[] = { 0, -1, 11, -2, 12, -3, 13, -4, 14, -5, 15, -6, 16, -7, 17, -8, 18, -9, 19, -10, 20 };
    int lenB = sizeof(arrB) / 4;
    err += test_quickSortArray(arrB, lenB);
    randomShuffle(arrB, lenB, 0);
    err += test_quickSortArray(arrB, lenB);

    std::string arrS[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    int lenS = 9;
    err += test_quickSortArray(arrS, lenS);
    randomShuffle(arrS, lenS, 0);
    err += test_quickSortArray(arrS, lenS);

    err += test_quickSortRand(99);

    printf("test_quickSort returning %d : %s\n", err, (err == 0 ? "PASS" : "FAIL"));
    return err;
}
