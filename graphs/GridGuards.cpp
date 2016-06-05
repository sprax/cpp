//
//  GridGuards.cpp
//
//  Created by Sprax Lines on 5/25/16.
//  Copyright © 2016 Sprax Lines. All rights reserved.
//

#include "stdafx.h"
#include <iostream>
#include <queue>

#include "Sx.h"
#include "GridGuards.hpp"

using namespace std;

static const char o = '-';
static const char G = 'G';
static const char x = 'X';

	typedef pair<int, int> square;


	GridGuards::GridGuards(const char *layout[], int rows, int cols) {
		mLayout = layout;
		mRows = rows;
		mCols = cols;
		mDistance = new int *[rows];
		for (int j = 0; j < mRows; j++)
			mDistance[j] = new int[cols];

		queue<square> marked;

		markGuards(marked);
		markDistances(marked);
	}

	void GridGuards::markGuards(queue<square> &marked)
	{
		for (int row = 0; row < mRows; row++) {
			for (int col = 0; col < mCols; col++) {
				char chr = mLayout[row][col];
				switch (chr) {
				case o:
					mDistance[row][col] = UNVISITED;
					break;
				case x:
					mDistance[row][col] = OBSTACLE;
					break;
				case G:
					mDistance[row][col] = 0;
					marked.push(square(row, col));
					break;
				default:
					throw invalid_argument("invalid layout");
				}
			}
		}
	}

	void GridGuards::markDistances(queue<square> &marked)
	{
		while (!marked.empty()) {
			square& sqr = marked.front();
			marked.pop();
			int row = sqr.first;
			int col = sqr.second;
			int dst = mDistance[row][col] + 1;

			// West (Left)
			col--;
			if (col >= 0 && mDistance[row][col] == UNVISITED) {
				addToMarked(row, col, dst, marked);
			}

			// East (Right)
			col += 2;
			if (col < mCols && mDistance[row][col] == UNVISITED) {
				addToMarked(row, col, dst, marked);
			}

			// North (Up)
			row--;
			col--;
			if (row >= 0 && mDistance[row][col] == UNVISITED) {
				addToMarked(row, col, dst, marked);
			}

			// South (Down)
			row += 2;
			if (row < mRows && mDistance[row][col] == UNVISITED) {
				addToMarked(row, col, dst, marked);
			}
		}
		//assert(marked.empty());
	}

	void GridGuards::addToMarked(int row, int col, int dst, queue<square> &marked) {
		mDistance[row][col] = dst;
		marked.push(square(row, col));
	}

	void GridGuards::showLayout()
	{
		Sx::putsArrayByPtrPtr2(mLayout, mRows, mCols);
	}

	void GridGuards::showDistance()
	{
		Sx::putsArrayByPtrPtr2((const int **)mDistance, mRows, mCols);
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
static void GridGuardsTest::putsArrayByPtr2(const T(*array)[sCols], int rows)
{
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < sCols; col++) {
			Sx::printOne(array[row][col]);
		}
		puts("");
	}
	puts("");
}

int GridGuardsTest::testGridGuards()
{
	cout << "Hello, GridGuards!\n";

	puts("putsArrayByRef2");
	Sx::putsArrayByRef2(sLayout);

	puts("putsArrayByPtrRef2");
	Sx::putsArrayByPtrRef2((const char(*)[][sCols]) &sLayout, sRows);

	puts("putsArrayByPtr2");
	putsArrayByPtr2((const char(*)[sCols]) &sLayout, sRows);

	puts("putsArrayByPtrPtr2");
	const char *rows[sRows];   // surrogate array of rows
	for (int j = 0; j < sRows; j++)
		rows[j] = sLayout[j];
	Sx::putsArrayByPtrPtr2(rows, sRows, sCols);

	const char *pRows[sRows];
	for (int j = 0; j < sRows; j++) {
		pRows[j] = sLayout[j];
	}
	GridGuards gg(pRows, sRows, sCols);
	gg.showLayout();
	cout << endl;
	gg.showDistance();

	return 0;
}


const char GridGuardsTest::sLayout[GridGuardsTest::sRows][GridGuardsTest::sCols] = {
	{ o, o, o, o, o, o, o, o, o, o },
	{ o, G, o, o, x, o, o, x, o, o },
	{ o, o, o, o, x, o, o, x, o, o },
	{ o, x, x, x, x, o, x, x, o, o },
	{ o, x, o, o, x, o, o, x, o, G },
	{ o, x, x, o, x, o, G, x, o, o },
	{ o, o, x, o, x, o, o, o, o, o },
	{ o, x, x, o, x, o, o, x, o, o },
	{ o, x, o, o, x, x, o, x, o, o },
	{ o, o, o, o, o, o, o, o, o, x },
	{ o, o, o, o, x, o, o, x, o, o },
	{ o, o, o, o, x, o, o, x, o, o },
};
