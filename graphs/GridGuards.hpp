//
//  GridGuards.cpp
//
//  Created by Sprax Lines on 5/25/16.
//  Copyright © 2016 Sprax Lines. All rights reserved.
//
#ifndef GridGuards_hpp
#define GridGuards_hpp

#include "stdafx.h"
#include <iostream>
#include <queue>

#include "Sx.h"

using namespace std;

class GridGuards
{
    typedef pair<int, int> square;

protected:
    int **mDistance;
    const int UNVISITED = -1, OBSTACLE = -2;
public:
    int mCols;
    int mRows;
    const char **mLayout;

	GridGuards(const char *layout[], int rows, int cols);
    ~GridGuards();

	void markGuards(queue<square> &marked);
	void markDistances(queue<square> &marked);
	void addToMarked(int row, int col, int dst, queue<square> &marked);
	void showLayout();
	void showDistance();
};





class GridGuardsTest
{
public:
	static const int sRows = 12;
	static const int sCols = 10;
	static const char sLayout[sRows][sCols];

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
	static void putsArrayByPtr2(const T(*array)[sCols], int rows);
	static int testGridGuards();

};

#endif