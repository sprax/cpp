// split_string.cpp : test if string is palindrome
// Requires C11 or later
// BUILD: clang++ -std=c++11 split_string.cpp -o tmpl.out && tmpl.out
// BUILD: clang++ -std=c++14 split_string.cpp -o tmpl.out && tmpl.out

#include <iostream>
#include <chrono>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <glob.h>
#include <signal.h>
#include <sstream>
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
    int test = 1;
    for (const char *lef = ss.c_str(), *rgt = lef + ss.size(); lef < --rgt; ) {
        printf("\nlef %c ? %c rgt\t", *lef, *rgt);
        if (*lef++ != *rgt) {
            int test = 2;
            return false;
        }
    }
    return true;
}

std::vector<std::string> split_string(const std::string& str, char sep = ',')
{
    std::vector<std::string> str_vec;
    std::string token;
    std::stringstream ss(str);
    while (std::getline(ss, token, sep))
    {
        str_vec.push_back(token);
    }
    return str_vec;
}


int main(int argc, char *argv[])
{
    const char *pch = argc > 1 ? argv[1] : "radar,sonar,guitar,redder";
    string str(pch);
    std::vector<std::string> strs = split_string(str);
    for (const string st : strs) {
        cout << st << "\t=>\t" << (is_palindrome(st) ? "TRUE" : "FALSE") << endl;
    }
    return 0;
}
