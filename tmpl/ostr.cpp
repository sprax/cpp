// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 ostr.cpp -o tmpl.out && tmpl.out
// BUILD: clang++ -std=c++14 ostr.cpp -o tmpl.out && tmpl.out

#include <iostream>
#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

class myclass
{
friend std::ostream& operator<<(std::ostream&, myclass const&);
public:
    myclass(int i) : k_(i) { }

private:
    int k_;
};

std::ostream &operator<<(std::ostream &os, myclass const &m) {
    return os << m.k_;
}

int main() {
    myclass x(10);

    std::cout << "myclass: " << x << std::endl;
    return 0;
}
