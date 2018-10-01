// test_cpp.cpp : test some inobvious features of C++
// BUILD: clang++ -std=c++11 -Wno-deprecated-register test_cpp.cpp -o tmpl.out && tmpl.out

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
#include <vector>

// #ifdef _DEBUG
// #include <assert.h>
// #else
// #define NDEBUG          1
// #define assert(_expr_)  ((void)0)
// #endif

using std::cerr;
using std::cout;
using std::endl;

using std::vector;

static const char *defProgramName  = "testC++";

// Substitution failure is not an error (SFINAE) refers to a situation in C++
// where an invalid substitution of template parameters is not in itself an error
struct Test
{
    typedef int Type;
};

template < typename T >
void f(typename T::Type) {} // definition #1

template < typename T >
void f(T) {}                // definition #2

// Problem: write an interface containing a template function that resolves
// to functions that only take an even (but variable) number of args.
// Hints below:

template<int I> void div(char(*)[I % 2 == 0] = 0) {
    /* this is taken when I is even */
}

template<int I> void div(char(*)[I % 2 == 1] = 0) {
    /* this is taken when I is odd */
}

void foo()
{
    f<Test>(10); // call #1
    f<int>(10);  // call #2 without error thanks to SFINAE: int is not a type with an nested type named Type,
                 //      but a valid function candidate remains after the compiler removes int (or "f<int>()")
                 //      from the pool of candidate functions, namely f<Test>().
}

template<typename T>
class MyInitList
{
};

#if 1
template<typename T>
class IsClassT {
  private:
    typedef char One;
    typedef struct { char a[2]; } Two;
    template<typename C> static One test(int C::*);
    // Will be chosen if T is anything except a class.
    template<typename C> static Two test(...);
  public:
    enum { Yes = sizeof(IsClassT<T>::test<T>(0)) == 1 };
    enum { No = !Yes };
};

// template<int N>
// struct Vector {
//     template<int M>
//     Vector(MyInitList<M> const& i,
//            typename enable_if_c<(M <= N)>::type* = 0) { /* ... */ }
// };

template<int N>
struct Vector {
    template<int M>
    Vector(MyInitList<int> const& i, char(*)[M <= N] = 0) { /* ... */ }
};
#endif


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

// blister b = naval;    /* error C2440: 'initializing' : cannot convert from 'orange' to 'blister' */
// int blister = naval;    /* error C2377: 'blister' : redefinition; typedef cannot be overloaded with any other symbol */
orange o    = naval; // blister; /* error: Cannot initialize type orange with int */


#ifdef _DEBUG
class Consty {
public:
    const int bar;
    Consty(int n) : bar(n)  {}
    Consty& operator=(const Consty& other) {
        Consty dude(other.bar);
        //(Consty*)this = &dude; // modifies a local copy?  The Consty on the LHS of the assignment is NOT changed. // warning C4213: nonstandard extension used : cast on l-value
        return *this;
    }

    // This compiles and runs but the assignment does not do what you might expect:
    static int testit() {
        Consty f( 44);
        Consty g(-55);
        Consty h = f;
        Consty i(g);
        g = f;
        return 0;
    }
};


// subclasses that inherit of inner classes
class AA {
public:
    class BB {
    public:
        BB() {}
    };

    AA() {}
};

class CC : public AA
{
    class DD : public BB {
        DD() {}
    };

    CC() {}
};

class EE {
    class FF : public AA::BB {
        FF() {}
    };
};


#endif

void sleep(unsigned millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

void count_it(int& count)
{
    if (count)
        count = 0;
    else
        count = 9999;
}


int main(int argc, char* argv[])    // NB: This is more a unit test than an app; it does not play ghost!
{
    static int counted;
    vector<double> vd { 1.1, 2.2, 3.3 };
    vector<double>& vr = vd;
    vr = { 9.9, 8.2 };
    register int regi;

    std::cout << vd[0] << std::endl;
    std::cout << vr[0] << std::endl;

    const char *progName = argv[0] ? argv[0] : defProgramName;
    const char *yourName = (argc > 0 && argv[1]) ? argv[1] : "No Name";
    const unsigned millis = 2222;
    bool just_test = false;
    if ( just_test ) {
#ifdef _DEBUG
        printf("Hi %s!  Hit <RETURN> to quit . . .\n", yourName);
        getchar();
#endif
        factorials();
        // printf("Just testing...  sleep %d milliseconds\n", millis);
        // sleep(millis);
        exit(0);    // zoid
    }

    std::vector<int> vec{1, 2, 3,};
    // std::vector<double> ret_vec, dbl_vec { 999.9, 888.8, 777.7, 666.6, 555.5 };
    // std::vector<int> vec{1, 2, 3,};

    std::string  str("my string");
    std::string& ref = str;
    cout << "str: " << str << endl;
    cout << "ref: " << ref << endl;
    ref = std::string("new");

    cout << "str: " << str << endl;
    cout << "ref: " << ref << endl;

    cerr << "counted: " << counted << endl;
    count_it(counted);
    cerr << "counted: " << counted << endl;

    int n = 44;
    printf( "n == %d \t &n = %p\n", n, &n);
    cout << "n == " << n << " \t " << "&n = " << std::hex << long(&n) << endl;

    regi = 0;
    return regi;
}
