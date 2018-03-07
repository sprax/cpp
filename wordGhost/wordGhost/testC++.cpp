// testC++.cpp : test some inobvious features of C++

#include <signal.h>
#include <typeinfo>

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include <thread>
#include <chrono>


#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
#include <assert.h>
#else
#define NDEBUG          1
#define assert(_expr_)  ((void)0)
#endif
#include <list>

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

#if 0
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

template<int N>
struct Vector {
    template<int M>
    Vector(MyInitList<M> const& i,
           typename enable_if_c<(M <= N)>::type* = 0) { /* ... */ }
};

template<int N>
struct Vector {
    template<int M>
    Vector(MyInitList<M> const& i, char(*)[M <= N] = 0) { /* ... */ }
};
#endif


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
        (Consty*)this = &dude; // modifies a local copy?  The Consty on the LHS of the assignment is NOT changed. // warning C4213: nonstandard extension used : cast on l-value
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

int main(int argc, char* argv[])    // NB: This is more a unit test than an app; it does not play ghost!
{
    const char *dictionaryFilePath = (argc > 0 && argv[1]) ? argv[1] : "Ha ha stitches";
    const unsigned millis = 2999;
    bool just_test = true;
    if ( just_test ) {
        printf("Just testing...  sleep %d milliseconds\n", millis);
#ifdef _DEBUG
        printf("Hit <RETURN> to quit . . .\n");
        getchar();
#endif
        sleep(millis);
        exit(0);    // zoid
    }
    return 0;
}
