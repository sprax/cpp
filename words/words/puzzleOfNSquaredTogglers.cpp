// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "tests.h"

int test_NlightBulbs(int N) 
{
    /* 
    There are 100 light bulbs lined up in a row in a long room. Each bulb has
    its own switch and is currently switched off. The room has an entry door 
    and an exit door. There are 100 people lined up outside the entry door. 
    Each bulb is numbered consecutively from 1 to 100. So is each person.

    Person No. 1 enters the room, switches on every bulb, and exits. 
    Person No. 2 enters and flips the switch on every second bulb (turning 
    off bulbs 2, 4, 6, …). 
    Person No. 3 enters and flips the switch on every third bulb (changing 
    the state on bulbs 3, 6, 9, …). 
    This continues until all 100 people have passed through the room.

    What is the final state of bulb No. 64? And how many of the light bulbs 
    are illuminated after the 100th person has passed through the room?  
    */
#define USE_MALLOC 0
#if     USE_MALLOC
    assert(0 < N && N < MaxN);
    bool *bulbs = (bool*)malloc(N*sizeof(bool));
    for (int i = 0; i < N; i++) { bulbs[i] = false; }
#else
    const int MaxN = 1024;
    bool bulbs[MaxN] = { false, };
#endif
    int countOn = 0;
    for (int j = 1; j <= N; j++) {
        printf("After %3d: ", j);
        for (int k = 0; k < N; k++) {
            if ((k+1) % j == 0) {
                bulbs[k] = ! bulbs[k];
                countOn += (bulbs[k] ? 1 : -1);
            }
            printf("%c", bulbs[k] ? '*' : '-');
        }
        printf(" %3d\n", countOn);
    }
    printf("On after %d:", N);
    for (int k = 0; k <= N; k++) {
        if (bulbs[k])
            printf("  %d", k+1);
    }
    printf("\n");
#if USE_MALLOC
    free(bulbs);
#endif
    return 0;
}

