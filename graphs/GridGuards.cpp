//
//  main.cpp
//  grids
//
//  Created by Sprax Lines on 5/25/16.
//  Copyright © 2016 Sprax Lines. All rights reserved.
//

#include "stdafx.h"
#include <iostream>

#include "Sx.h"

//void putsArray(const char *preLabel, int A[], int len);

class GridGuards
{
public:
    static const char o = '-';
    static const char G = 'G';
    static const char x = 'X';
    static const size_t wide = 10;
    static const size_t tall = 12;

    const static char floor[tall][wide];
    static int one[];


    static void showFloor()
    {
        for (int j = 0; j < tall; j++) {
            for (int k = 0; k < wide; k++) {
                std::cout << floor[j][k];
            }
            std::cout << std::endl;
        }
    }

    static void printit()
    {
        int AA[] = { 1, -4, 9 };
        Sx::putsArray(one, 3);
    }
};

const char GridGuards::floor[GridGuards::tall][wide] = {
    { o, o, o, o, o, o, o, o, o, o },
    { o, G, o, o, x, o, o, x, o, o },
    { o, o, o, o, x, o, o, x, o, o },
    { o, x, x, x, x, o, x, x, o, o },
    { o, x, o, o, x, o, o, x, o, G },
    { o, x, x, o, x, o, G, x, o, o },
    { o, o, x, o, x, o, o, o, o, o },
    { o, x, x, o, x, o, o, x, o, o },
    { o, x, o, o, x, o, o, x, o, o },
    { o, o, o, o, o, o, o, o, o, x },
    { o, o, o, o, x, o, o, x, o, o },
    { o, o, o, o, x, o, o, x, o, o },
}; 

int GridGuards::one[] = { 1, -2, 3 };

/** 
* print a 2d array passed by reference.
* This may work only for automatic arrays (usually abiding on the stack)
* whose dimensions are known at compile time (and thus can be used
* to evaluate the Rows and Cols template arguments).
* For dynamic (freestore) arrays on the heap, it may not compile.
*/
template <typename T, size_t Rows, size_t Cols>
void putsArrayByRef2(const T(&array)[Rows][Cols])
{
    for (size_t row = 0; row < Rows; row++) {
        for (size_t col = 0; col < Cols; col++) {
            Sx::printOne(array[row][col]);
        }
        puts("");
    }
    puts("");
}

/** 
* print a 2d array passed by as a pointer to the array of templatized width (number
* of columns per row).
* This may work only for automatic arrays or an array of pointers to automatic
* arrays (one for each row) where the width (number of columns) is known at compile time.
*/
template <typename T, size_t Cols>
void putsArrayByPtrRef2(const T(*array)[][Cols], size_t rows)
{
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < Cols; col++) {
            Sx::printOne((*array)[row][col]);
        }
        puts("");
    }
    puts("");
}

/**
* print a 2d array by passing (by value) a pointer to the decayed type
* of the array, which is a pointer to a variable number of rows of known
* constant width.  This width must be a constant specified in the original
* declaration of the calling argument array, or in a cast of a pointer to
* that array (so that one way or another, the width is specified at compile time).
* Since the argument type does not specified the number of rows, this number 
* must be passed in to the method as a separate argument.  The compiler can
* only check that the number of columns matches that specified in the 
* second dimension of the argument type.
*/
template <typename T>
void putsArrayByPtr2(const T(*array)[GridGuards::wide], size_t rows)
{
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < GridGuards::wide; col++) {
            Sx::printOne(array[row][col]);
        }
        puts("");
    }
    puts("");
}

/**
* print a 2d array by passing a pointer to a pointer, taking advantage
* of the decay of arrays into pointers (which C++ "inherited" from C, 
* i.e. allows for backward compatibility with C).  This method is not
* safe for actual 2d arrays, but is guaranteed to work with a 1D array 
* of pointers to 1D arrays (the rows as separate arrays).  Due to array 
* flattening or memory padding/alignment, the offsets to each row might 
* not be the expected multiple of the number of columns.  That is, while
* some compilers might let you get away with casting a 2D array to a 
* pointer to a pointer (as in (const T **)&array[0][0]), the only portably
* safe way to call this method is to define an auxiliary array of pointers
* to each rows (as in T *X[Rows]; for (int r = 0; r < Rows; r++) X[r] = A[r];), 
* and then to pass this auxiliary array as the argument to the method.
* See the tests below.
*/
template <typename T>
void putsArrayByPtrPtr2(const T *array[], size_t rows, size_t cols)
{
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            Sx::printOne(array[row][col]);
        }
        puts("");
    }
    puts("");
}


int testGridGuards()
{
    std::cout << "Hello, GridGuards!\n";
    GridGuards::printit();
    GridGuards::showFloor();
    std::cout << std::endl;

    puts("putsArrayByRef2");
    putsArrayByRef2(GridGuards::floor);

    puts("putsArrayByPtrRef2");
    putsArrayByPtrRef2((const char(*)[][GridGuards::wide]) &GridGuards::floor, GridGuards::tall);

    puts("putsArrayByPtr2");
    putsArrayByPtr2((const char(*)[GridGuards::wide]) &GridGuards::floor, GridGuards::tall);

    puts("putsArrayByPtrPtr2");
    const char *rows[GridGuards::tall];   // surrogate array of rows
    for (size_t j = 0; j < GridGuards::tall; j++)
        rows[j] = GridGuards::floor[j];

    putsArrayByPtrPtr2(rows, GridGuards::tall, GridGuards::wide);

    /****
    try {
        putsArrayByPtrPtr2((const char **)&GridGuards::floor[0][0], GridGuards::tall, GridGuards::wide);
    }
    catch (...) {
        Sxs::put("exception caught");
    }
    ****/


    //Sxc::printArray2(&(GridGuards::floor), GridGuards::tall, GridGuards::wide);
    return 0;
}
