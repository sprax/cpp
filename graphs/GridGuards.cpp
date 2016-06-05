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
GridGuards::~GridGuards() {
    for (int j = 0; j < mRows; j++)
        delete[] mDistance[j];
    delete[] mDistance;
}

void GridGuards::markGuards(queue<square> &marked)
{
    for (int row = 0; row < mRows; row++) {
        for (int col = 0; col < mCols; col++) {
            char chr = mLayout[row][col];
            switch (chr) {
                case Empty:
                    mDistance[row][col] = UNVISITED;
                    break;
                case Obstacle:
                    mDistance[row][col] = OBSTACLE;
                    break;
                case Guard:
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
