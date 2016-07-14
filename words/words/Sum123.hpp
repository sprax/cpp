#ifndef Sum123_hpp
#define Sum123_hpp

#include "stdafx.h"
#include "BinLink.hpp"

typedef unsigned long long int ullint;

/** 
 * Sum123: compute the number of ways to add 1's, 2's, and 3's up to a specified sum.
 */
class Sum123
{
public:
    // Eventually the result will exceed the following (from limits.h):
    // #define ULLONG_MAX    0xffffffffffffffffui64       /* maximum unsigned long long int value */
    static const  int   maxSum  = 76;   // Actually, ULLONG overflow will occur at sum == 75.
    static ullint saved[maxSum];
    static    int maxSaved;

    static ullint numOnesTwosAndThreesToSum_naive(int sum)
    {   // naive recurseive algorithm
        if (sum  < 0L)
            return 0L;
        if (sum == 0L)
            return 1L;
        return numOnesTwosAndThreesToSum_naive(sum - 1)
             + numOnesTwosAndThreesToSum_naive(sum - 2)
             + numOnesTwosAndThreesToSum_naive(sum - 3);
    }

    static ullint numOnesTwosAndThreesToSum_saved(int sum)
    {   // memoized recursive algorithm
        if (sum  < 0)
            return 0;
        if (sum < maxSaved)
            return saved[sum];
        if (sum >= maxSum)
            return -1;      // error on ullint overflow
        ullint wts = numOnesTwosAndThreesToSum_saved(sum - 3)
                   + numOnesTwosAndThreesToSum_saved(sum - 2)
                   + numOnesTwosAndThreesToSum_saved(sum - 1);
        saved[sum] = wts;
        maxSaved   = sum;
        return wts;
    }

    static int unit_test()
    {
        for (int  sum = 0; sum < Sum123::maxSum + 2; sum += 1) {
            ullint wts = Sum123::numOnesTwosAndThreesToSum_saved(sum);
            printf("numOnesTwosAndThreesToSum_saved(%2u) is %llu\n", sum, wts);
            if (sum > 0 && wts < Sum123::saved[sum-1]) {
                printf("ERROR: result at %u is less than previous one; overflow already happened!\n", sum);
                break;
            } else if (wts > ULLONG_MAX / 2) {
                printf("WARNING: result exceeds ULLONG_MAX/2 (%llu); overflow lurks...\n", ULLONG_MAX/2);
            }
            if (sum < 28) { // naive way is painfully slow after about 30
                ullint slo = Sum123::numOnesTwosAndThreesToSum_naive(sum);
                printf("numOnesTwosAndThreesToSum_naive(%2u) is %llu\n", sum, slo);
            }
        }
        return 0;
    }

};

#endif // Sum123_hpp