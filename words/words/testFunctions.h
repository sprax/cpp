// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#ifndef testFunctions_h
#define testFunctions_h

typedef bool (*boolFunc_intArray)(int A[], size_t len);
typedef bool (*boolFunc_1int)(int);
typedef bool (*boolFunc_2int)(int,int);

//  test_<return type>FunctionType<Args>_<domain>
int test_boolFunctionOneInt_MinMax( boolFunc_1int boolFunc, int min, int max);



#endif // testFunctions_h