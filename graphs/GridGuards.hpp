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

class GridGuards
{
public:
    typedef std::pair<int, int> square;

    static const char Empty = '-';
    static const char Guard = 'G';
    static const char Obstacle = 'X';

    int mCols;
    int mRows;
    const char **mLayout;

	GridGuards(const char *layout[], int rows, int cols);
    virtual ~GridGuards();

	void showLayout();
	void showDistance();

protected:
    void markGuards(std::queue<square> &marked);
    void markDistances(std::queue<square> &marked);
    void addToMarked(int row, int col, int dst, std::queue<square> &marked);

    int **mDistance;
    const int UNVISITED = -1, OBSTACLE = -2;
};

#endif