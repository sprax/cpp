// numberical.cpp

#include "stdafx.h"
#include "tests.h"


/*
A shop sells an item in packets of 6, 9, and 17. 
A customer can buy any number of packets, but a packet cannot be broken up. 
Provide an efficient algorithm to solve the question "Can I buy N items?".
For example, is it possible to buy 29 items? What about 19? Your algorithm 
simply needs to return true or false.
*/

bool multiple_of_6_or_9_naive(int n)
{
    // Assuming n >= 6, so we don't need: if (n < 6) return false;
    if (n % 3 == 0) {
        int m = n / 3;
        if (m % 2 == 0 || m % 3 == 0)
            return true;
    }
    return false;
}


bool divisibe_6_9_smart(int n)
{
    // Assuming n >= 6, so we don't need: if (n < 6) return false;
    if (n % 3 == 0)
        return true;
    return false;
}


bool divisible_6_9_17(int n)
{
    if (n < 6)
        return false;
    // if n >= 6 and n % 3 == 0, then n == j*6 + k*9 for some j, k > 0.  So only test n % 3.  
    if (n % 3 == 0 || n % 17 == 0)
        return true;
    return (divisible_6_9_17(n - 6) || divisible_6_9_17(n - 9) || divisible_6_9_17(n - 17));
}

int test_divisible_6_9_17(int min, int max)
{
    int count = 0;
    for (int j = min; j < max; j++) {
        if (divisible_6_9_17(j)) {
            count++;
            printf(" %2d", j);
        }
    }
    puts("\n");
    return count;
}

bool divisible_6_11_19(int n)
{
    if (n < 6)
        return false;
    if (n % 6 == 0 || n % 11 == 0 || n % 19 == 0)
        return true;
    return (divisible_6_11_19(n - 6) || divisible_6_11_19(n - 11) || divisible_6_11_19(n - 17));
}
