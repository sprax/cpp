// Sorts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MergeSort.h"
#include "MergeSortSinLink.h"
#include "QuickSort.h"
#include "SortTests.h"

int test_sorts(int level)
{
    int error = 0;

    error += test_mergeSortSinLink(0);
    error += test_quickSort(0);
    error += test_mergeSort(0);
    error += test_arraySorts(0);

    test_sortsNaive(level);

    return error;
}

int main(int argc, char* argv[])
{
    int error = test_sorts(0);
	//getchar(); // modify console window properties while waiting for input
	return error;
}
