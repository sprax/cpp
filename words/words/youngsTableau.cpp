// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "tests.h"

static int sDbg = 1;

//////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

/*Finding kth minimum in the matrix */
int J0[]  = {  1,  2,  3, 14, 15 };
int J1[]  = {  4,  5,  6, 24, 25 };
int J2[]  = {  7,  8,  9, 34, 35 };
int J3[]  = { 10, 11, 43, 44, 45 };
int J4[]  = { 12, 13, 53, 54, 55 };

int *JJ[] = { J0, J1, J2, J3, J4 };  // median is 13 at ordinal(5,2) -- the last possible possition!

/*
int KK[][] =
{ {  1,  2,  3, 14, 15, 16 }
, {  4,  5,  6, 17, 23, 26 }
, {  7,  8,  9, 18, 35, 36 }
, { 10, 11, 43, 44, 45, 46 }
, { 12, 13, 53, 54, 55, 56 } 
, { 62, 63, 63, 64, 65, 66 } 
};
*/

/*returns the number of elements which are smaller than or equal to x in the given matrix */
int numSmaller(int *a[], int nRows, int nCols, int x)
{
    int nSmaller = 0;
    for (int row = nRows-1, col = 0; row >= 0 && col < nCols; ) {
        if (a[row][col] <= x) {
            nSmaller += (row+1);
            col++;
        } else {
            row--;
        }
    }  
    return nSmaller;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int bin_search(int *a[], int nRows, int nCols, int start, int last, const int &k)
{
    if(start > last) 
        return a[nRows-1][nCols-1]+1;

    int mid = (start + last) >> 1;       // average of first and last values
    int x = numSmaller(a, nRows, nCols, mid); 
    // numSmaller(...x) returns the number of elements which are 
    // smaller than or equal to x in the given matrix
    if(x == k) 
        return min(mid, bin_search(a, nRows, nCols, start, mid-1, k));
    else if(x>k) 
        return bin_search(a, nRows, nCols, start, mid-1, k);
    else return bin_search(a, nRows, nCols, mid+1, last, k);
}


int test_bs(int k)
{
    int **AA = JJ;
    printf("size JJ:  %d   size J0:  %d   size AA  %d\n", sizeof(JJ), sizeof(J0), sizeof(AA) );
    printf("AA(%x) == JJ(%x), but sizeof(AA)(%ud) != sizeof(JJ)(%d)\n"
        , (unsigned int)AA, (unsigned int)JJ, sizeof(AA), sizeof(JJ) );
    //int nRows = sizeof(AA)
    
    int nRows = 5, nCols = 5;
    int n = 5, bs = -1;
    for (k = 1; k < 24; k++) {
        bs = bin_search(JJ, nRows, nCols, JJ[0][0], JJ[n-1][n-1], k);
        printf("test_bs (%d) got %d\n", k, bs);
    }
    return bs;
}



//////////////////////////////////////////////////////////////////
/////////////////// YOUNG's TABLEAU //////////////////////////////
//////////////////////////////////////////////////////////////////

int find_n(int x[][3],int row, int col, int n) {  

    int r =0 , c = col -1;  
    while( r < row && col >=0) {  
        if(x[r][c] == n)  
            return n;  
        else if ( x[r][c] > n)  
            c--;  
        else  
            r++;  
    }  
    return -1;  
}

/**
AA is young tableau, row and col are current position in the tableau that we are filling 
nRows and nCols are number of rows and column 
*/  
void youngify(int *AA[], int row, int col, int nRows, int nCols, int bigVal) 
{
    if (row > nRows || col > nCols) 
        return;

    int x = -1, y = -1;
    // If the value directly below is greater than the one at row, col, 
    // then move it up, and place the "infinite" value in its old place.
    if (row+1 < nRows && AA[row][col] > AA[row+1][col]) {
        AA[row][col] = AA[row+1][col];
        x = row+1;
        y = col;
    }
    // Then, also, If the value directly right is greater than the one at row, col, 
    // then move it left, and place the "infinite" value in its old place.
    // Actually, it does not matter which neighbor you check and move first,
    // but both of them must handled. (Two if-then's, not one if-else-then.)
    if (col+1 < nCols && AA[row][col] > AA[row][col+1]) {
        AA[row][col] = AA[row][col+1];
        x = row;
        y = col+1;
    }
    if (x != -1) {
        AA[x][y] = bigVal;
        youngify(AA, x, y, nRows, nCols, bigVal);
    }
}


/*
Inserting an element into non full young tableau follows the similar process
as extracting minimum element. The idea is to put the element into last row 
and column and move it upto the position where is should be.

Sorting can also be implemented for n X n young tableau in O(n ^3) time by
using the operation by which we extract minimum element. The idea is to 
extract minimum element which takes O(n+n) = O(n) time, IF we repeat this 
for n^2 element it will take O (n ^3)time. 
*/
//////////////////////////////////////////////////////////////////

static const int NN = 4;
/*
int find_kth_largest (int A[][NN], int k)
{
    std::priority_queue<int,int> m_heap;
    //max_heap_t m_heap;
    std :: pair <int, int> next_largest;
    m_heap . insert (make_pair <int, int> (0, 0));
    int count = 0;

    while (1)
    {
        next_largest = m_heap . extract ();
        if (++count == k)
            break;

        m_heap . insert (make_pair <int, int> (next_largest.first, next_largest.second + 1));
        m_heap . insert (make_pair <int, int> (next_largest.first + 1, next_largest.second));
    }

    return A[next_largest.first][next_largest.second];
}
*/
