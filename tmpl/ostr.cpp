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
    const double sqrt2 = 1.41;
};

std::ostream &operator<<(std::ostream &os, myclass const &m) {
    return os << m.k_ << " sqrt2: " << m.sqrt2;
}

int main() {
    myclass xx(10);
    myclass *my_ptr = NULL;

    std::cout << "myclass: " << xx << std::endl;
    time_t tt = time(nullptr);
    std::cout << "my time: " << tt << std::endl;
    delete my_ptr;

    return 0;
}
