// Sx : convenience class (namespace) for static output functions
// Sprax Lines 2016 
// Based on Sx.java.  Sx.cs exists as well.

#include "stdafx.h"

#include "Sx.h" // Not needed here because it's in stdafx.h

int Sx::unit_test()
{
    std::cout << "BEGIN Sx unit_test" << std::endl;

    const size_t rows = 3;
    const size_t cols = 4;
    const char charArray[rows][cols] = {
        { 'A', 'B', 'C', 'D' },
        { 'E', 'F', 'G', 'H' },
        { 'I', 'J', 'K', 'L' },
    };

    puts("putsArrayByRef2");
    Sx::putsArrayByRef2(charArray);

    puts("putsArrayByPtrRef2");
    Sx::putsArrayByPtrRef2((const char(*)[][cols]) charArray, rows);

    puts("putsArrayByPtr2");
    Sx::putsArrayByPtr2((const char(*)[][cols]) &charArray, rows, cols);

    puts("putsArrayByPtrPtr2");
    const char *rowArray[rows];   // surrogate array of rows
    for (size_t j = 0; j < rows; j++)
        rowArray[j] = charArray[j];
    Sx::putsArrayByPtrPtr2(rowArray, rows, cols);

    /****
    try {
    putsArrayByPtrPtr2((const char **)&GridGuards::floor[0][0], GridGuards::tall, GridGuards::wide);
    }
    catch (...) {
    Sxs::put("exception caught");
    }
    ****/

    std::cout << std::endl;
    std::cout << "END   Sx unit_test" << std::endl;
    return 0;
}