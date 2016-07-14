// MergeSort.h

#pragma once

template <typename T>
void mergeSort(T array[], unsigned int length);

template <typename T>
void mergeArrayLeftRight(T array[], T temp[], unsigned int lo, unsigned int mid, unsigned int hi);


int test_mergeSort(int level);

