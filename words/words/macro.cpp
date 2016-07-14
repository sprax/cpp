// macro.cpp
#include "stdafx.h"

#include "tests.h"

#define prn(a) printf("%d",a)
#define print(a,b,c) prn(a), prn(b), prn(c)
#define max(a,b) ((a)>(b) ? (a) : (b))

int test_macro()
{
  int x=1, y=2;
  print(max(x++,y),x,y);
  print(max(x++,y),x,y);
  return 0;
}

