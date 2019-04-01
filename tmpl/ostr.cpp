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
using std::string;
using std::vector;
class myclass
{
friend std::ostream& operator<<(std::ostream&, myclass const&);

public:
    myclass(int i) : k_(i) { }

private:
    int k_;
    const double sqrt2 = 1.41;
};

std::ostream &operator<<(std::ostream& os, myclass const& m) {
    return os << m.k_ << " sqrt2: " << m.sqrt2;
}

std::string endpath_after_dir(const std::string& path, const std::string& dir)
{
    size_t beg = 0, pos = path.find(dir, 1);
    if (pos == string::npos) {
        return "";
    }
    beg = 1 + pos + dir.size();
    std::string endpath = path.substr(beg);
    cout << "find " << dir << " in " << path << " gives pos: " << pos << " so beg = " << beg << endl;
    cout <<  "Thus the endpath is: (" << endpath << ")" << endl;
    return endpath;
}

int main() {
    myclass xx(10);
    myclass *my_ptr = NULL;

    std::cout << "myclass: " << xx << std::endl;
    time_t tt = time(nullptr);
    std::cout << "my time: " << tt << std::endl;
    delete my_ptr;


    string path("/homes/s2/usr/sprax/traj_lib/truffle/digger/get_it");
    string lib_dir("traj_lib");
    size_t beg = 0, pos = path.find(lib_dir, 1);
    if (pos != string::npos) {
        beg = 1 + pos + lib_dir.size();
        string subpath = path.substr(beg);
        cout << "find " << lib_dir << " in " << path << " gives pos: " << pos << " so beg = " << beg << endl;
        cout <<  "Thus the subpath is: (" << subpath << ")" << endl;
    }

    string endpath = endpath_after_dir(path, lib_dir);
    cout << "FINALLY endpath: (" << endpath << ")" << endl;

    double pi;
    if ((pi = M_PI) && (pi > 3)) {
        cout << "M_PI == " << pi << " is > 3" << endl;
    }

    auto a = std::make_shared<int>(42);
    auto b = a;
    *a = 43;
    cout << "*b after *a = 43 is now: " << *b << endl;
    (*a.get())++;
    cout << "*b after (*a.get())++ is now: " << *b << endl;
    ++*a;
    cout << "*b after ++*a is now: " << *b << endl;

    assert(false && "asserted false");
    return 0;
}
