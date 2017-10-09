// stacks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int test_ocvDisplay(const char *imgFileName);
int test_ocvRandomRows(const char *imgFileName);
int test_ocvRandomRows(int nCols, int nRows);

using namespace std;

static void stackGrowsUpOrDownLocalVariables()
{
	int a = 1;
	int b = 2;
	cout << "Variables: Stack grows " << ((&b - &a) > 0 ? "UP" : "DOWN") << endl;
}

static void stackGrowsUpOrDownFunctionCalls(int *pa)
{
	int b = 0;
	cout << "Functions: Stack grows " << ((&b - pa) > 0 ? "UP" : "DOWN") << endl;
}


// stack_pop.cpp
// compile with: /EHsc
#include <stack>
#include <iostream>

int test_std_stack( )
{
   using namespace std;
   stack <int> s1, s2;

   s1.push( 10 );
   s1.push( 20 );
   s1.push( 30 );

   stack <int>::size_type i;
   i = s1.size( );
   cout << "The stack length is " << i << "." << endl;

   i = s1.top( );
   cout << "The element at the top of the stack is "
        << i << "." << endl;

   s1.pop( );

   i = s1.size( );
   cout << "After a pop, the stack length is " 
        << i << "." << endl;

   i = s1.top( );
   cout << "After a pop, the element at the top of the stack is "
        << i << "." << endl;

   return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 0;
	stackGrowsUpOrDownFunctionCalls(&a);
	stackGrowsUpOrDownLocalVariables();
    test_std_stack();
	
 //   const char *imgFileName = "LakeLouiseW.jpg";
   // int stat = test_ocvDisplay(imgFileName);
   // const char *imgFileNameB = "TaranakiH.jpg";   
    const char *imgFileNameB = "C:\\asd\\PIX\\TaranakiM400.jpg";
//    test_ocvRandomRows(imgFileNameB);


////test_ocvRandomRows(64, 32);

    getchar();    
    return 0;
}
