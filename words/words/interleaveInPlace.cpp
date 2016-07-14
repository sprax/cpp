// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"

#include "tests.h"

using namespace std;

int a[100];


void Swap(int * x, int * y) {
    int z = *x;
    *x = *y;
    *y = z;
}


void Swap(int p, int q, int m) {
    for (int i=0; i<m; i++) {
        Swap(&a[p+i], &a[q+i]);
    }
}


void Work(int p, int n) {
    //a[p]...a[p+3*n-1]

    if (n == 1) 
        return;

    int m = n/2;

    Swap(p+n+m, p+2*n, m);
    Swap(p+m, p+n, m);
    Swap(p+n, p+n+m, m);

    if (n % 2 == 1) {
        int x = a[p+n-1];
        int y = a[p+2*n-1];

        int q = p+n-1;
        for (int i=n; i<=2*n-2; i++) 
            a[q++] = a[p+i];
        for (int i=2*n; i<=3*n-2; i++) 
            a[q++] = a[p+i];

        a[p+3*n-3] = x;
        a[p+3*n-2] = y;

        Work(p, m);
        Work(p+n+m-1, m);
    } else {
        Work(p, m);
        Work(p+n+m, m);
    }


}

void interleaveRecursive(int n)
{
    Work(0, n);
}

int test_interleaveInPlace() {
    int n = 9;
    for (int i=0; i<n*3; i++) 
        a[i] = i;
    interleaveRecursive(n);
    for (int i=0; i<n*3; i++) 
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}

