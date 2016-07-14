// sortsNaive.cpp
// Sprax Lines

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <functional>      // For greater<int>( )
#include <iostream>

// Adapted from MS alg_sort.cpp
// compile with: /EHsc




inline static void swap(int array[], int j, int k)
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

static void quickSort(int array[], unsigned int length)
{
	if (array == NULL || length < 2)
		return;

	quickSortRec(array, 0, length - 1);
}

static void test_quickSort()
{
	int array[] = { 0, -1, 2, -3, 4, -5, 6, -7, 8, -9 };
	int length = sizeof(array) / 4;
	quickSort(array, length);
	printf("quickSorted: ");
	printArray(array, length);
}



// Return true/false indicating whether first element is greater than the second
bool UDgreater(int elem1, int elem2)
{
	return elem1 > elem2;
}

struct LetNum {
	LetNum(char c, int n) : mLet(c), mNum(n) {}
	char mLet;
	int  mNum;
};

struct LetNumLess {
	bool operator()(LetNum const& a, LetNum const& b) const {
		return a.mNum < b.mNum;
	}
};

// compare mNum values of 2 pointers to LetNum specified as array elements
int qsort_s_LetNum(void *arr, const void *arg1, const void *arg2)
{
	return (*(LetNum*)arg1).mNum - ((LetNum*)arg2)->mNum;
}

int test_sorts()
{
	using namespace std;
	vector <int> v1;
	vector <int>::iterator Iter1;


	test_quickSort();

	int i;
	for (i = 0; i <= 5; i++)
	{
		v1.push_back(2 * i);
	}

	int ii;
	for (ii = 0; ii <= 5; ii++)
	{
		v1.push_back(2 * ii + 1);
	}

	cout << "Original vector v1 = ( ";
	for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
		cout << *Iter1 << " ";
	cout << ")" << endl;

	sort(v1.begin(), v1.end());
	cout << "Sorted vector v1 = ( ";
	for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
		cout << *Iter1 << " ";
	cout << ")" << endl;

	// To sort in descending order. specify binary predicate
	sort(v1.begin(), v1.end(), greater<int>());
	cout << "Resorted (greater) vector v1 = ( ";
	for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
		cout << *Iter1 << " ";
	cout << ")" << endl;

	// A user-defined (UD) binary predicate can also be used
	sort(v1.begin(), v1.end(), UDgreater);
	cout << "Resorted (UDgreater) vector v1 = ( ";
	for (Iter1 = v1.begin(); Iter1 != v1.end(); Iter1++)
		cout << *Iter1 << " ";
	cout << ")" << endl;

	vector<LetNum> vln;
	vln.push_back(LetNum('c', 2));
	vln.push_back(LetNum('e', 4));
	vln.push_back(LetNum('d', 3));
	vln.push_back(LetNum('a', 0));
	vln.push_back(LetNum('b', 1));

	sort(vln.begin(), vln.end(), LetNumLess());
	cout << "Std::sorted vector of LetNum: vln = ( ";
	for (vector<LetNum>::iterator it = vln.begin(); it != vln.end(); it++)
		cout << it->mLet << " " << it->mNum << ", ";
	cout << ")" << endl;

	LetNum letNum = vln.at(3);

	LetNum aln[5] = { vln.at(2), vln.at(1), vln.at(4), vln.at(3), vln.at(0) };
	qsort_s(aln, 5, sizeof(LetNum), qsort_s_LetNum, NULL);
	cout << "clib QSorted array of LetNum: aln = ( ";
	for (int j = 0; j < 5; j++)
		cout << aln[j].mLet << " " << aln[j].mNum << ", ";
	cout << ")" << endl;

	return 0;
}


void test_naiveCstringAddressCompare()
{
    printf("\n   Applied to C-strings, the less-than operator < only compares their memory addresses...\n");
    bool less;
    char *pStrB = "BBBB";
    char *pStrA = "AAAA";    // pStrA will have a higher address than pStrB
    less = pStrA < pStrB;
    printf("Is %s(%x) < %s(%x) ? %d\n\n", pStrA, pStrA, pStrB, pStrB, less);
    less = ((long)pStrA >> 1) < (long)pStrB;
    printf("Is (%s >> 1)(%x) < %s(%x) ? %d\n\n", pStrA, (unsigned long)pStrA >> 1, pStrB, pStrB, less);
}

void test_naiveStdStringCompare()
{
    printf("\n   Applied to std::strings, the less-than operator < compares their content lexicographically...\n");
    bool less;
    std::string *pStrB = new std::string("CDFBB");
    std::string *pStrA = new std::string("CDAAA");    // pStrA will have a higher address than pStrB
    less = *pStrA < *pStrB;
    printf("Is %s(%x) < %s(%x) ? %d\n\n", pStrA->c_str(), pStrA, pStrB->c_str(), pStrB, less);
   // less = ((long)pStrA >> 1) < (long)pStrB;
   // printf("Is (%s >> 1)(%x) < %s(%x) ? %d\n\n", pStrA, (unsigned long)pStrA >> 1, pStrB, pStrB, less);
}

void test_sortsNaive(int level = 1)
{
    if (level < 1)
        return;
    test_naiveCstringAddressCompare();
    test_naiveStdStringCompare();
}