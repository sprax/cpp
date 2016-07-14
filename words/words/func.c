
#include "stdafx.h"
#include "stdio.h"

// extern "C" {
    int func()
    {
        printf("\ncrap");
    }
//}

int funk()
{
    return printf("\ncrap");
}

int test_fun()
{
    printf("\n return value of func %d", func());
    printf("\n return value of funk %d", funk());
    return 0;
}
