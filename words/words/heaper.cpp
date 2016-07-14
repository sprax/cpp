
// heapfunc.cpp
// compile with: /EHsc
// 
// Functions:
//    make_heap : convert a sequence to a heap
//    sort_heap : sort a heap
//    push_heap : insert an element in a heap
//    pop_heap  : remove the top element from a heap

// disable warning C4786: symbol greater than 255 characters,
// okay to ignore
#pragma warning(disable: 4786)

#include "stdafx.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

int test_heaper()
{
    const int VECTOR_SIZE = 8 ;

    // Define a template class vector of int
    typedef vector<int > IntVector ;

    //Define an iterator for template class vector of strings
    typedef IntVector::iterator IntVectorIt ;

    IntVector Numbers(VECTOR_SIZE) ;

    IntVectorIt it ;

    // Initialize vector Numbers
    Numbers[0] = 4 ;
    Numbers[1] = 10;
    Numbers[2] = 70 ;
    Numbers[3] = 10 ;
    Numbers[4] = 30 ;
    Numbers[5] = 69 ;
    Numbers[6] = 96 ;
    Numbers[7] = 100;

    // print content of Numbers
    cout << "Numbers { " ;
    for(it = Numbers.begin(); it != Numbers.end(); it++)
        cout << *it << " " ;
    cout << " }\n" << endl ;

    // convert Numbers into a heap
    make_heap(Numbers.begin(), Numbers.end()) ;

    cout << "After calling make_heap\n" << endl ;

    // print content of Numbers
    cout << "Numbers { " ;
    for(it = Numbers.begin(); it != Numbers.end(); it++)
        cout << *it << " " ;
    cout << " }\n" << endl ;

    // sort the heapified sequence Numbers
    sort_heap(Numbers.begin(), Numbers.end()) ;

    cout << "After calling sort_heap\n" << endl ;

    // print content of Numbers
    cout << "Numbers { " ;
    for(it = Numbers.begin(); it != Numbers.end(); it++)
        cout << *it << " " ;
    cout << " }\n" << endl ;

    //insert an element in the heap
    //Numbers.push_back(7) ;
    //push_heap(Numbers.begin(), Numbers.end()) ;
    make_heap(Numbers.begin(), Numbers.end()) ;

    // you need to call make_heap to re-assert the
    // heap property
    make_heap(Numbers.begin(), Numbers.end()) ;

    cout << "After calling push_heap and make_heap\n" << endl ;

    // print content of Numbers
    cout << "Numbers { " ;
    for(it = Numbers.begin(); it != Numbers.end(); it++)
        cout << *it << " " ;
    cout << " }\n" << endl ;

    // remove the root element from the heap Numbers
    pop_heap(Numbers.begin(), Numbers.end()) ;

    cout << "After calling pop_heap\n" << endl ;

    // print content of Numbers
    cout << "Numbers { " ;
    for(it = Numbers.begin(); it != Numbers.end(); it++)
        cout << *it << " " ;
    cout << " }\n" << endl ;

    return 0;
}
