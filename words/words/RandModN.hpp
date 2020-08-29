#ifndef RandModN_hpp
#define RandModN_hpp

#include "stdafx.h"
#include "BinLink.hpp"

typedef unsigned long long int ullint;

/** 
 * RandModN: compute the number of ways to add 1's, 2's, and 3's up to a specified sum.
 */
class RandModN
{
public:
    static int rand5() { return rand() % 5; }  // uniform distribution from 0 to 4

    static int rand7_from_rand5_a() {
        // From 5*rand5() + rand5(), there is only one way any of the numbers in the range [0, 25).
        // Now, [0, 21) is 3 * [0, 7), which means that [0, 21) % 7 includes each N in [0, 7) exactly 
        // 3 times.  
        int  num;
        do { num = rand5() + rand5()*5; 
        } while (num > 20);
        return num % 7;
    }

    static int rand7_from_rand5_b() {
        // From 5*rand5() + rand5(), there is only one way to get each of the numbers in the range [0, 25).
        // Now, [0, 21) is 3 * [0, 7), which means that [0, 21) % 7 includes each N in [0, 7) exactly 
        // 3 times.  
        for (;;) {
            int num = rand5() + rand5()*5;
            if (num < 21)
                return num % 7;
        }
    }

    static int unit_test()
    {
        int factor = 1000000, counts[7] = { 0, };   // initialize all counts to 0
        srand(clock());
        for (int j = 0; j < 7*factor; j++) {
            int num = rand7_from_rand5_b();
            counts[num]++;
        }
        int dif, maxDif = 0, sum = 0;
        printf("RandModN::unit_test:\n");
        printf("If the distribution is uniform, the following counts should all be about the same:\n");
        for (int j = 0; j < 7; j++) {
            int count = counts[j];
            sum += count;
            dif = factor - count;
            if (dif < 0)
                dif = -dif;
            if (maxDif < dif)
                maxDif = dif;
            printf("count[%d] == %5d\n", j, counts[j]);
        }
        printf("sum %d,  maxDif %d,  fraction %.8f\n", sum, maxDif, (double)maxDif/factor);
        return 0;
    }

};

#endif // RandModN_hpp
