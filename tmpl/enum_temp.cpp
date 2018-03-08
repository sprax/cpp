// enum_temp.cpp : template classes using enum values as template parameters

#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

using std::cin;
using std::cout;
using std::endl;

template <int N>
struct Factorial
{
     enum { value = N * Factorial<N - 1>::value };
};

template <>
struct Factorial<0>
{
    enum { value = 1 };
};

// Factorial<4>::value == 24
// Factorial<0>::value == 1
void factorials()
{
    int x = Factorial<4>::value; // == 24
    int y = Factorial<5>::value; // == 1
    int z = Factorial<0>::value; // == 1
    std::cout << "x = fac(4) == " << x << std::endl;
    std::cout << "y = fac(5) == " << y << std::endl;
    std::cout << "z = fac(0) == " << z << std::endl;
}

typedef enum { blood, blue  } blister;
typedef enum { naval, seedless, /*blood*/ } orange;  /* blood gets a redefinition error; previous def was 'enumerator' */

typedef enum tagMotionType {
    SCOOP_TYPE_1 		= 0,
    SCOOP_TYPE_2 		= 1,
    NUM_MOTION_TYPES 	= 2
} MotionType;

template<MotionType M>
class Mover
{
public:
    void showMover() {
        std::cout << "Mover of MotionType " << M << std::endl;
    }
};

int main(int argc, char* argv[])    // NB: This is more a unit test than an app; it does not play ghost!
{
    const unsigned millis = 2222;
    factorials();
    Mover<SCOOP_TYPE_2> mover2;
    mover2.showMover();

    return 0;
}
