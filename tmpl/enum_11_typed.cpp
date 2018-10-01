/// @file: enum_11_typed.cpp : C++11 enum examples
/// @From: https://stackoverflow.com/questions/28828957/enum-to-string-in-modern-c11-c14-c17-and-future-c20?noredirect=1&lq=1
/// Requires C11 or later
/// BUILD: clang++ -std=c++11 enum_11_typed.cpp -o tmpl.out && tmpl.out
/// BUILD: clang++ -std=c++14 enum_11_typed.cpp -o tmpl.out && tmpl.out

#include "enum_11_typed.h"

// #include <chrono>
// #include <list>
// #include <set>
// #include <signal.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <typeinfo>
// #include <unordered_map>
// #include <unordered_set>

// #include <algorithm>
// #include <iostream>
// #include <map>
// #include <sstream>
// #include <string>
// #include <vector>

using std::cin;
using std::cout;
using std::endl;


DECLARE_ENUM_WITH_TYPE(TestEnumClass, int32_t, ZERO = 0x00, TWO = 0x02, ONE = 0x01, THREE = 0x03, FOUR);

void example_enum_with_type()
{
    TestEnumClass first, second;
    first = TestEnumClass::FOUR;
    second = TestEnumClass::TWO;

    cout << endl;
    cout << "Example: DECLARE_ENUM_WITH_TYPE(TestEnumClass, int32_t, ZERO = 0x00, TWO = 0x02, ...);"
         << endl;

    std::cout   << first << "(" << static_cast<uint32_t>(first) << ")" << std::endl; // FOUR(4)

    std::string strOne;
    strOne = ~first;
    std::cout << strOne << std::endl; // FOUR

    std::string strTwo;
    strTwo = ("Enum-" + second) + (TestEnumClass::THREE + "-test");
    std::cout << strTwo << std::endl; // Enum-TWOTHREE-test

    std::string strThree("TestEnumClass: ");
    strThree += second;
    std::cout << strThree << std::endl; // TestEnumClass: TWO
    std::cout << "Enum count=" << *first << std::endl;

    TestEnumClassMapName;
}

///////////////////////////////////////////////////////////////////////////////

#define SequentialEnum(Name, ...)                                   \
enum Name { __VA_ARGS__ };                                          \
namespace {                                                         \
    const std::initializer_list<Name> Name##List { __VA_ARGS__ };   \
    const std::string Name##Array[] { #__VA_ARGS__ };               \
};

SequentialEnum(Shape,
    Circle,
    Square,
    Triangle,
    Oval,
    Polygon
);
// FIXME: duplication is not DRY

const std::string ShapeNames[]{ "Circle", "Square", "Triangle", "Oval", "Polygon"};
// std::vector<Shape> zoosh ShapeList;


void example_sequential_enum()
{
    cout << endl;
    cout << "Example: SequentialEnum(Shape, Circle, Square, Triangle, Oval, Polygon);"
         << endl;

    cout << "Oval: " << Shape::Oval << "  |  ShapeNames: " << ShapeNames[Shape::Oval] << endl;
    for (auto ss : ShapeList) {
        cout << "Shape enum val from list: " << ss << "   name from array: " << ShapeArray[ss] << endl;
    }
    // cout << "ShapeList: " << ShapeList[0] << endl;
}



int main(int argc, char* argv[])    // NB: This is more a unit test than an app; it does not play ghost!
{
    example_enum_with_type();
    example_sequential_enum();
    return 0;
}
