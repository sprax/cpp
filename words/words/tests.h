// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef tests_h
#define tests_h

#include "testFunctions.h"

//       bitfield.cpp
int test_bitvector();

// divisible.cpp
typedef bool (*boolFunc_1int)(int);
typedef bool (*boolFunc_2int)(int,int);
int test_boolFunctionOneInt_MinMax( boolFunc_1int boolFunc, int min, int max);
int test_divisible_6_9_17(int min, int max);
bool divisible_6_9_17(int n);
bool divisible_6_11_19(int n);

//      3dtransformations.cpp
int test_3dtransformations(int argc, char **argv);

//       charString.cpp
int test_charString();

//       fibonacci.cpp
int test_fibmemo();
int test_fibonacci();
int test_fibonacci_viscomp();

int test_for_each();
int test_rotatedString();
int test_threeWayPartition();

//       puzzleOfNSquaredTogglers.cpp
int test_NlightBulbs(int N = 100);

//       slicingProblem.cpp
int test_slicingProblem();

int test_interleaveInPlace();
int test_uniqueCharSubString();
int test_macro();

int test_multimap();

// numerical.cpp
double sqrt_bisection(double t); // simple binary search (not Newton-Raphson)

// searchInfinite.cpp
bool binary_search_infinite_array(char *infA, char x);

//       numerical.cpp
int test_sqrt_bisection();

// sequences.cpp
bool isArrayConsecutiveConst(int A[], size_t len);   // Do not modify A[]
bool isArrayConsecutiveModSwap(int A[], size_t len); // can modify A[]
bool isArrayConsecutiveMod_bust(int A[], int len); // can modify A[]
int test_isArrayConsecutive(boolFunc_intArray isConsecutive, int len, int offset);

int test_heaper();
int test_sorts( );

#endif // tests_h