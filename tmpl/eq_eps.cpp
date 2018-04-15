// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 traj_map_.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 traj_map_.cpp -o tmpl && tmpl

#include <iostream>
#include <chrono>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;


/** Determins if two values are 'close enough' based on a scaled tolerance */
template<typename T>
bool eq_eps(T a, T b, T rel_epsison = std::numeric_limits<T>::epsilon())
{
    T value_range[] = {T(1.0), a, b};
    // FIXME: elminated the + 3 below:
    if (fabs(a - b) <= rel_epsison * *std::max_element(value_range, value_range + 3))
    {
        return true;
    }
    return false;
}

template<typename T>
T median_odd(std::vector<T> vec)
{
    const auto median_it = vec.begin() + vec.size() / 2;
    std::nth_element(vec.begin(), median_it , vec.end());
    return *median_it;
}

/** returns median value of even-length vector */
template<typename T>
T median_even(std::vector<T> vec)
{
    const auto median_it1 = vec.begin() + int(vec.size() / 2) - 1;
    const auto median_it2 = vec.begin() + int(vec.size() / 2);
    assert(median_it1 == median_it2-1);
    return (*median_it1 + *median_it2) / 2;
}

/** returns median value of any-length vector.  But the template type must be floating_point
* (float, double, long double) or it will not compile (as in: `error: no matching function`).
*/
template<typename T>
T median(std::vector<T> vec, typename std::enable_if<std::is_floating_point<T>::value, T>::type* = 0)
{
    if (vec.size() == 0) {
        return std::numeric_limits<T>::signaling_NaN();
    }
    return (vec.size() % 2) ? median_odd(vec) : median_even(vec);
}


int main(int argc, char* argv[])    // NB: unit tests for MapTraj
{
    return 0;
}
