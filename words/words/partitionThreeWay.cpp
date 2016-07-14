// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "tests.h"

static void swapTemp(int &a, int &b)
{
    int t = a;
    a = b;
    b = t;
}

static void swapAddSubtract(int &a, int &b) {
    a = a + b;
    b = a - b;
    a = a - b;
}

static void swapXor(int &a, int &b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
}

static int test_swaps(int a, int b) // not by reference
{
    int aa = a, bb = b, err = 0; // could use abs(bb - a) + abs(aa - b);
    swapTemp(aa, bb);
    if (aa != b || bb != a) {
        printf("ERROR: swaptTemp(%d, %d) --> (%d, %d)\n", a, b, aa, bb);
        err += 1;
    }
    aa = a, bb = b;
    swapAddSubtract(aa, bb);
    if (aa != b || bb != a) {
        printf("ERROR: swapAddSubtract(%d, %d) --> (%d, %d)\n", a, b, aa, bb);
        err += 2;
    }
    aa = a, bb = b;
    swapXor(aa, bb);
    if (aa != b || bb != a) {
        printf("ERROR: swapXor(%d, %d) --> (%d, %d)\n", a, b, aa, bb);
        err += 4;
    }
    return err;
}

// Dutch Flag Algorithm: unstable partitioning around 2 pivots
void threeWayPartitionDutchFlag(int data[], int len, int pivot1, int pivot2)
{
    int p = 0;                              // p is the size of the low partition.
    int q = len;                            // len - q is the size of the high partition.
    for (int j = 0; j < q;) {
        if (data[j] < pivot1) {             // data[j] is to go into the low partition.
            swapTemp(data[j], data[p]);     // order is not generally preserved in the low partition (data[j] may have been swapped from the end).
            ++j;                            // increment j, no more testing below here.
            ++p;                            // increment boundary of low partition.
        } else if (data[j] >= pivot2) {     // data[j] is to go into the high partition
            --q;                            // decrement q, but don't increment j; data[j] must be re-tested.
            swapTemp(data[j], data[q]);     // order is not generally preserved in the high partition (unstable, similarly to low partition).
        } else {
            ++j;                            // data[j] is to remain in the middle partition (also unstable)
        }
    }
}

// unstable partitioning mod 3
void threeWayPartitionMod3(int data[], int len)
{
    int p = 0;                              // p is the size of the low partition.
    int q = len;                            // len - q is the size of the high partition.
    for (int j = 0; j < q;) {
        if (data[j]%3 < 1) {             // data[j] is to go into the low partition.
            swapTemp(data[j], data[p]);     // order is not generally preserved in the low partition (data[j] may have been swapped from the end).
            ++j;                            // increment j, no more testing below here.
            ++p;                            // increment boundary of low partition.
        } else if (data[j]%3 > 1) {     // data[j] is to go into the high partition
            --q;                            // decrement q, but don't increment j; data[j] must be re-tested.
            swapTemp(data[j], data[q]);     // order is not generally preserved in the high partition (unstable, similarly to low partition).
        } else {
            ++j;                            // data[j] is to remain in the middle partition (also unstable)
        }
    }
}


// Attempt at stable partitioning around 2 pivots
void threeWayPartitionStable(int data[], int len, int pivot1, int pivot2)
{
    // Algorithm:
    // First pass: Visit entire array, swapping only into the low partition, counting elements of each value.  
    //     Did the displaced values ramain in order?   No.
    // Second pass: Starting after the 1st partition, swap
    //
    //
    int p = 0;                              // p is the size of the low partition.
    int q = len;                            // len - q is the size of the high partition.
    /*
    for (int j = 0; j < q;) {
        if (data[j] < pivot1) {             // data[j] is to go into the low partition.
            swapTemp(data[j], data[p]);     // order is not generally preserved in the low partition (data[j] may have been swapped from the end).
            ++j;                            // increment j, no more testing below here.
            ++p;                            // increment boundary of low partition.
        } else if (data[j] >= pivot2) {     // data[j] is to go into the high partition
            --q;                            // decrement q, but don't increment j; data[j] must be re-tested.
            swapTemp(data[j], data[q]);     // order is not generally preserved in the high partition (unstable, similarly to low partition).
        } else {
            ++j;                            // data[j] is to remain in the middle partition (also unstable)
        }
    }
    */
}



int test_missing_number()
{
    int vA[] = { 1, 2, 4, 3, 5, 6, 8, 7, 9, 10 };
    int vB[] = { 1, 2, 4, 3, 5, 6, 8,    9, 10 };
    int sizeA = sizeof(vA)/sizeof(int);  // only works for local arrays
    int sizeB = sizeA - 1;
    int xA = vA[0];
    for (int j = 1; j < sizeA; j++) {
        xA ^= vA[j];
    }   
    int xB = vB[0];
    for (int j = 1; j < sizeB; j++) {
        xB ^= vB[j];
    }
    int missing = xA ^ xB;
    printf("test_missing_number got %d\n", missing);
    return missing;
}

int test_threeWayPartition()
{
    int x = 5;
    int y = 7;
    int z = 9;

    int a = 123456, b = 876543;
    test_swaps(a, b);
    test_missing_number();

    assert(x < y && y < z);
    int data[] = { y, x, y, y, x, z, y, x, y, z, x, x, x, y, y, z, y, x };
    int length = sizeof(data)/sizeof(int);    // only works on local array
    int pivot1 = x + 1;
    int pivot2 = z;
    assert(x < pivot1 && pivot1 <= y);      // ok if pivot1 == y
    assert(y < pivot2 && pivot2 <= z);      // ok if pivot2 == z
    printf("threeWayPartition(data, %d, %d, %d):\t", length, pivot1, pivot2);
    threeWayPartitionDutchFlag(data, length, pivot1, pivot2);
    for (int j = 0; j < length; j++) {
        printf("%d ", data[j]);
    }
    printf("\n");

    printf("threeWayPartitionMod3(data, %d):\t", length);
    threeWayPartitionMod3(data, length);
    for (int j = 0; j < length; j++) {
        printf("%d ", data[j]);
    }
    printf("\n");

    int data3[21];
    for (int k = 21; --k >= 0; )
        data3[k] = k;
    printf("threeWayPartitionMod3(data3, %d):\t", length);
    threeWayPartitionMod3(data3, length);
    for (int j = 0; j < length; j++) {
        printf("%d ", data3[j]);
    }
    printf("\n");
    return 0;
}

