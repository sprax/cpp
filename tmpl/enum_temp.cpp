// enum_temp.cpp : template classes using enum values as template parameters

#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
    FIRST_MotionType    = 0,
    SCOOP_TYPE_1 		= 0,
    SCOOP_TYPE_2 		= 1,
    NUM_MotionTypes 	= 2
} MotionType;





template<MotionType M>
class Mover
{
public:
    void showMover() {
        std::cout << "Mover of MotionType " << M << std::endl;
    }
};


template<MotionType M>
class SpecMover : public Mover<M>
{
public:
    void showMover() {
        cout << "Mover of MotionType " << M << endl;
    }
};


void listMotionTypes()
{
    #if __cplusplus <= 199711L
    static const int arr[] = {16,2,77,29};
    std::vector<int> int_vec (arr, arr + sizeof(arr) / sizeof(arr[0]) );
    static const MotionType mta[] = {SCOOP_TYPE_1, SCOOP_TYPE_2};
    std::vector<MotionType> template_arg_vec (mta, mta + sizeof(mta) / sizeof(mta[0]) );
    #else
    // NOTE: g++ -std=c++11   OR   clang++ -std=c++11
    std::vector <int> int_vec = {1, 2, 3, 4, 5};
    std::vector<MotionType> template_arg_vec = {SCOOP_TYPE_1, SCOOP_TYPE_2};
    #endif
    for (int j = FIRST_MotionType; j < NUM_MotionTypes; j++) {
        MotionType typer = template_arg_vec[j];
        printf("MotionType(%d)\n", typer);
        // const int tt = static_cast<int>(typer); // fails because value is not known at compile time
        // Mover<tt> mover;         // error: non-type template argument of type 'int' is not an integral constant expression
        Mover<SCOOP_TYPE_1> mover;
        mover.showMover();
    }
}

#include "enum_temp.h"
#include "enum_temp.h"

/// The kinds of files we read and write, in order of preference:
namespace my_ns {
    /// Please keep the FileFormatExts parallel to FileFormat enum values.
    extern const char * const FileFormatExts[] = { (char *const) ".mpac"
                                                 , (char *const) ".bin"
                                                 , (char *const) ".json"
                                                 , (char *const) ""
    };
}   // namespace my_ns


int main(int argc, char* argv[])
{
    const unsigned millis = 2222;

    for (int j = my_ns::FIRST_FILE_FORMAT; j < my_ns::NUM_FILE_FORMATS + 1; j++) {
        printf("int j: %d    enum: %d    extn: <%s>\n"
              , j
              , static_cast<my_ns::FileFormat>(j)
              , my_ns::FileFormatExts[j]
        );
    }

    return 1;


    factorials();
    listMotionTypes();

    return 0;
}
