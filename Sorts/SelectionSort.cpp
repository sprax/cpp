// SelectionSort.cpp : sprax 2015.07
#include "stdafx.h"
#include <stdio.h>
//#include <stdlib.h>
#include <assert.h>
//#include <time.h>
//#include <signal.h>
//#include <limits.h>
#include "sortUtil.h"
#include "SelectionSort.h"






// This is here only to "trick" the compiler into generating the
// code for SelectionSortSort<int>::selectionSort(...), etc., so the
// linker can find it by its named address, as in passing the 
// following argument into a function: &(InsertionSort<int>::selectionSort)
// Without it, the linker gets a fatal error: unresolved external...
SelectionSort<int> *createSelectionSortInt()
{
#if 0
    auto inst = new SelectionSort<int>();
    assert(inst);
    return inst;
#else
    return NULL;
#endif
}