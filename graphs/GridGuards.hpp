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

typedef pair<int, int> square;

class GridGuards
{
public:
    static const char Empty = '-';
    static const char Guard = 'G';
    static const char Obstacle = 'X';

    int mCols;
    int mRows;
    const char **mLayout;

	GridGuards(const char *layout[], int rows, int cols);
    ~GridGuards();

	void showLayout();
	void showDistance();

protected:
    void markGuards(queue<square> &marked);
    void markDistances(queue<square> &marked);
    void addToMarked(int row, int col, int dst, queue<square> &marked);

    int **mDistance;
    const int UNVISITED = -1, OBSTACLE = -2;
};

#endif