// numberical.cpp

#include "stdafx.h"
#include "tests.h"

static const double ERROR = 0.000000001;

double sqrt_bisection( double t ) // simple binary search (not Newton-Raphson)
{
    if (t < 0.0)
        t = -t;     // or throw an error

    double low = 0;
    double high = t > 1.0 ? t : 1.0;
    for (double mid; (high - low) > ERROR; ) {
        mid = ( low + high ) / 2.0;
        if (mid*mid > t ) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return low;
}

int test_sqrt_bisection()
{
    double sqrtBisection;
    double sqrtMathLib;
    double td[] = { 16, 10, 9, 4, 2, 1, 0.5, 0.1, 0.0, -1.0, -2 };
    int len = sizeof(td)/sizeof(double), stat = 0;
    for (int j = 0; j < len; j++) {
        double dd = td[j];
        sqrtBisection = sqrt_bisection(dd);
        sqrtMathLib = sqrt(dd >= 0.0 ? dd : -dd);
        if (abs(sqrtBisection - sqrtMathLib) > ERROR*2.0)
            stat -= 1;
    }
    return stat;
}