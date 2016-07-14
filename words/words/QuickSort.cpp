// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"

#include "QuickSort.h"

//#include <vector>
//#include <algorithm>
//#include <functional>      // For greater<int>( )
//#include <iostream>


static void swap(int array[], int j, int k)
{
	int temp = array[j];
	array[j] = array[k];
	array[k] = temp;
}

static int quickPartition(int array[], int lo, int hi)
{
	int beg = lo;
	int end = hi + 1;
	int val = array[lo];
	while (true)
	{
		while (array[++beg] < val && beg < hi);
		while (array[--end] > val && end > lo);
		if (beg >= end)
			break;
		swap(array, beg, end);
	}
	swap(array, lo, end);
	return end;
}


static void quickSortRec(int array[], int lo, int hi)
{
	if (lo >= hi)
		return;

	int part = quickPartition(array, lo, hi);
	quickSortRec(array, lo, part - 1);				// sort left partition
	quickSortRec(array, part + 1, hi);				// sort right partition
}

void quickSort(int array[], unsigned int length)
{
	if (array == NULL || length < 2)
		return;

	quickSortRec(array, 0, length - 1);
}

int test_quickSort()
{
	int array[] = { 0, -1, 2, -3, 4, -5, 6, -7, 8, -9 };
	int length = sizeof(array) / 4;
	quickSort(array, length);
	printf("quickSorted: ");
	printArray(array, length);
	return 0;
}



