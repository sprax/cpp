// NitWit.cpp : unfinished pseudo iterator for pairs of ints
// Sprax Lines,  July 2010

#include "NitWit.hpp"

static inline int minint(const int a, const int b) { return a < b ? a : b; }
static inline int maxint(const int a, const int b) { return a < b ? b : a; }

// zoid: not ready for export!
static bool nextWideTall_A(int &wide, int &tall, const int &minWide, const int &minTall, const int &maxWide, const int &maxTall, bool ascending)
{
    if (ascending) {
        if (wide < 0 && tall < 0) {     // special case: initialize
            wide = tall = minTall;      // no minWide, becase we only try wide >= tall
            return minWide <= wide && wide <= maxWide;
        }
        if (wide < maxWide) {
            wide++;
            return true;
        }
        if (tall < maxTall) {
            tall++;
            wide = tall;
            return true;
        }
    } else {
        if (wide < 0 && tall < 0) {     // special case: initialize
            wide = maxWide;             // yes maxWide
            tall = maxTall;
            return wide >= tall;
        }
        if (wide > tall) {
            wide--;
            return true;
        }
        if (tall > minTall) {
            tall--;
            wide = maxWide;
            return true;
        }
    }
    return false;
}



static int nextWideTall_B(int& wide, int& tall, const int& minTall, const int& maxTall, const int& minArea, const int& maxArea, const int& maxWide, bool ascending)
{
    if (ascending) {
        if (tall < 0) {                     // special case:
            tall = wide = minTall;          // initialize
        } else if (wide < maxWide) {
            wide++;
        } else {
            tall++;
            wide = tall;
        }
        for ( ; tall <= maxTall ; ++tall, wide = tall ) {
            for ( ; wide <= maxWide ; wide++ ) {
                int area = wide*tall;
                if (minArea <= area && area <= maxArea) {
                    return area;
                }
            }
        }
    } else {
        if (wide < 0 && tall < 0) {     // special case: initialize...
            wide = maxWide;              
            tall = maxTall;             // no special checking: the case maxWide < maxTall falls through to 0
        } else if (wide > tall) {
            wide--;
        } else {
            tall--;
            wide = maxWide;
        }
        for ( ; tall >= minTall ; --tall, wide = maxWide ) {
            for ( ; wide >= tall ; wide-- ) {
                int area = wide*tall;
                if (minArea <= area && area <= maxArea) {
                    return area;
                }
            }
        }
    }
    return 0;     // default: no suitable values for wide & tall remain
}


static int test_it() 
{
    int    minTall = 5;
    int    maxTall = 9;
    int    minArea = 53;
    int    maxArea = 65;
    int minWide = 2,  maxWide = 20;
    int wide=4444, tall=2222;

    printf("\n  B ascending:\n");
    int j = 0;
    for (wide = -1, tall = -1; nextWideTall_B(wide, tall, minTall, maxTall, minArea, maxArea, maxWide, true) ; ) {
        printf("WT %3d: %3d %3d  ==  %3d\n", ++j, wide, tall, wide*tall);
    }

    j = 0;
    printf("\n  B descending:\n");
    for (wide = -1, tall = -1; nextWideTall_B(wide, tall, minTall, maxTall, minArea, maxArea, maxWide, false) ; ) {
        printf("WT %3d: %3d %3d  ==  %3d\n", ++j, wide, tall, wide*tall);
    }

    return 0;
}

