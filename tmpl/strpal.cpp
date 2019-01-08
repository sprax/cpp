// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 strpal.cpp -o tmpl.out && tmpl.out
// BUILD: clang++ -std=c++14 strpal.cpp -o tmpl.out && tmpl.out

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
using std::string;

bool is_palindrome(const std::string& ss)
{
    for (const char *lef = ss.c_str(), *rgt = lef + ss.size(); lef < --rgt; ) {
        printf("\nlef %c ? %c rgt\t", *lef, *rgt);
        if (*lef++ != *rgt) {
            return false;
        }
    }
    return true;
}


int main() {

    string strs[]{ "radar", "sonar", "guitar", "redder" };
    for (const string st : strs) {
	cout << st << "\t=>\t" << is_palindrome(st) << endl;
    }
    return 0;
}
