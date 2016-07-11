// testSystem.cpp : show system parameters
//

#include "stdafx.h"

static void testSizeofTypes()
{
    printf("sizeof char & char *:  %d  %d\n", sizeof(char), sizeof(char *) );
    printf("sizeof short & short *:  %d  %d\n", sizeof(short), sizeof(short *) );
    printf("sizeof int  &  int *:  %d  %d\n", sizeof(int), sizeof(int *) );
    printf("sizeof long & long *:  %d  %d\n", sizeof(long), sizeof(long *) );
    printf("sizeof long long & long long *:  %d  %d\n", sizeof(long long), sizeof(long long *) );
    printf("sizeof float & float *:  %d  %d\n", sizeof(float), sizeof(float *) );
    printf("sizeof double & double *:  %d  %d\n", sizeof(double), sizeof(double *) );
    printf("sizeof long double & long double *:  %d  %d\n", sizeof(long double), sizeof(long double *) );
}

