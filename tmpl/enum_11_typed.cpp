/// @file: enum_11_typed.cpp : C++11 enum examples
/// @From: https://stackoverflow.com/questions/28828957/enum-to-string-in-modern-c11-c14-c17-and-future-c20?noredirect=1&lq=1
/// Requires C11 or later
/// BUILD: clang++ -std=c++11 enum_11_typed.cpp -o tmpl.out && tmpl.out
/// BUILD: clang++ -std=c++14 enum_11_typed.cpp -o tmpl.out && tmpl.out

// #include <chrono>
// #include <list>
// #include <set>
// #include <signal.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <typeinfo>
// #include <unordered_map>
// #include <unordered_set>

#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;


#define STRING_REMOVE_CHAR(str, ch) str.erase(std::remove(str.begin(), str.end(), ch), str.end())

std::vector<std::string> splitString(std::string str, char sep = ',') {
    std::vector<std::string> vecString;
    std::string item;

    std::stringstream stringStream(str);

    while (std::getline(stringStream, item, sep))
    {
        vecString.push_back(item);
    }

    return vecString;
}

#define DECLARE_ENUM_WITH_TYPE(E, T, ...)                                                                     \
    enum class E : T                                                                                          \
    {                                                                                                         \
        __VA_ARGS__                                                                                           \
    };                                                                                                        \
    std::map<T, std::string> E##MapName(generateEnumMap<T>(#__VA_ARGS__));                                    \
    std::ostream &operator<<(std::ostream &os, E enumTmp)                                                     \
    {                                                                                                         \
        os << E##MapName[static_cast<T>(enumTmp)];                                                            \
        return os;                                                                                            \
    }                                                                                                         \
    size_t operator*(E enumTmp) { (void) enumTmp; return E##MapName.size(); }                                 \
    std::string operator~(E enumTmp) { return E##MapName[static_cast<T>(enumTmp)]; }                          \
    std::string operator+(std::string &&str, E enumTmp) { return str + E##MapName[static_cast<T>(enumTmp)]; } \
    std::string operator+(E enumTmp, std::string &&str) { return E##MapName[static_cast<T>(enumTmp)] + str; } \
    std::string &operator+=(std::string &str, E enumTmp)                                                      \
    {                                                                                                         \
        str += E##MapName[static_cast<T>(enumTmp)];                                                           \
        return str;                                                                                           \
    }                                                                                                         \
    E operator++(E &enumTmp)                                                                                  \
    {                                                                                                         \
        auto iter = E##MapName.find(static_cast<T>(enumTmp));                                                 \
        if (iter == E##MapName.end() || std::next(iter) == E##MapName.end())                                  \
            iter = E##MapName.begin();                                                                        \
        else                                                                                                  \
        {                                                                                                     \
            ++iter;                                                                                           \
        }                                                                                                     \
        enumTmp = static_cast<E>(iter->first);                                                                \
        return enumTmp;                                                                                       \
    }                                                                                                         \
    bool valid##E(T value) { return (E##MapName.find(value) != E##MapName.end()); }

#define DECLARE_ENUM(E, ...) DECLARE_ENUM_WITH_TYPE(E, int32_t, __VA_ARGS__)
template <typename T>
std::map<T, std::string> generateEnumMap(std::string strMap)
{
    STRING_REMOVE_CHAR(strMap, ' ');
    STRING_REMOVE_CHAR(strMap, '(');

    std::vector<std::string> enumTokens(splitString(strMap));
    std::map<T, std::string> retMap;
    T inxMap;

    inxMap = 0;
    for (auto iter = enumTokens.begin(); iter != enumTokens.end(); ++iter)
    {
        // Token: [EnumName | EnumName=EnumValue]
        std::string enumName;
        T enumValue;
        if (iter->find('=') == std::string::npos)
        {
            enumName = *iter;
        }
        else
        {
            std::vector<std::string> enumNameValue(splitString(*iter, '='));
            enumName = enumNameValue[0];
            //inxMap = static_cast<T>(enumNameValue[1]);
            if (std::is_unsigned<T>::value)
            {
                inxMap = static_cast<T>(std::stoull(enumNameValue[1], 0, 0));
            }
            else
            {
                inxMap = static_cast<T>(std::stoll(enumNameValue[1], 0, 0));
            }
        }
        retMap[inxMap++] = enumName;
    }

    return retMap;
}


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
    example_sequential_enum();
    example_enum_with_type();
    return 0;
}
