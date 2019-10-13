// palstr.cpp : test if string is palindrome
/* Requires C11 or later
clang++ -std=c++11 palstr.cpp -o tmpl.out && tmpl.out
clang++ -std=c++14 palstr.cpp -Wall -Wextra -o tmpl.out && tmpl.out
*/

#include <chrono>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cerr;
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

/// const int duh() { return 2; } --> warning: 'const' type qualifier on return type has no effect [-Wignored-qualifiers]
int duh() { return 2; }

int main() {

    string strs[]{ "radar", "sonar", "guitar", "redder", "" };
    for (const string st : strs) {
	    cout << "[" << st << "]\t=>\t" << (is_palindrome(st) ? "True" : "False") << endl;
    }
    return 0;

    auto ans = duh();
    ans = 4;
    cerr << "ans: " << ans << endl;

    int iflag = 0;
    iflag |= -1;
    cerr << "iflag 0 |= -1: " << iflag << endl;
    iflag = iflag | -2;
    cerr << "iflag 0 |= -2: " << iflag << endl;
    iflag |= -4;
    cerr << "iflag 0 |= -4: " << iflag << endl;
    iflag |= +4;
    cerr << "iflag 0 |= +4: " << iflag << endl;
    iflag += +4;
    cerr << "iflag 0 += +4: " << iflag << endl;

    unsigned int uflag = 0;
    uflag |= 1;
    cerr << "uflag 0 |= 1: " << uflag << endl;
    uflag = uflag | 2;
    cerr << "uflag 0 |= 2: " << uflag << endl;
    uflag |= 4;
    cerr << "uflag 0 |= 4: " << uflag << endl;

    iflag = 0;
    iflag |= 1;
    cerr << "iflag 0 |= 1: " << iflag << endl;
    iflag = iflag | 2;
    cerr << "iflag |= 2: " << iflag << endl;
    iflag |= 4;
    cerr << "iflag |= 4: " << iflag << endl;
    iflag |= +8;
    cerr << "iflag |= +8: " << iflag << endl;
    iflag |= +8;
    cerr << "-iflag: " << -iflag << endl;

    iflag = -15;
    std::ostringstream oss;
    oss << "Badness :";
    if (iflag & -8)
        oss << " AtoE";
    if (iflag & -4)
        oss << " EtoF";
    cerr << "err_msg: " << oss.str() << endl;

    for (int j = 100; j > -5; j--) {
        int r = rand();
        if (j & -1 || r & -1) {
            cerr << "j & -1: " << (j&-1) << " and r & -1: " << (r&-1) << endl;
        }
    }

    return 0;
}
