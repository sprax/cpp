// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 traj_map_.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 traj_map_.cpp -o tmpl && tmpl

#include <chrono>
#include <cmath>
#include <iostream>
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

#include <boost/range/combine.hpp>

#ifdef HAVE_EIGEN
#include <Eigen/Dense>
#else
namespace Eigen {
    typedef std::vector<double> VectorXd;
}
#endif

using std::cin;
using std::cout;
using std::endl;
using std::string;


/// Determins if two values are 'close enough' based on a scaled tolerance.
/// NOTE: for integral types, the numeric_limits<T>::epsilon() will return 1.
template<typename T>
bool eq_eps( T a, T b, T rel_epsilon = std::numeric_limits<T>::epsilon()
           , typename std::enable_if<std::is_floating_point<T>::value, T>::type* = 0)
{
    T value_range[] = {T(1.0), a, b};
    return (std::abs(a - b) <= rel_epsilon * *std::max_element(value_range, value_range + 3));
}

template<typename T>
bool eq_eps( T a, T b, T rel_epsilon = std::numeric_limits<T>::epsilon()
           , typename std::enable_if<std::is_integral<T>::value, T>::type* = 0)
{
    T value_range[] = {T(1), a, b};
    T limit = rel_epsilon * *std::max_element(value_range, value_range + 3);
    cout << "  rel_epsilon::::::: " << rel_epsilon
         << "  limit: " << limit
         << "  fabs(a - b): " << fabs(a - b)
         << "  std::abs(a - b): " <<  std::abs(a - b)
         << "  value_range: [ "
         << value_range[0] << ", "
         << value_range[1] << ", "
         << value_range[2] << " ]" << endl;

    if (std::abs(a - b) <= limit)
    {
        return true;
    }
    return false;
}


bool eq_eps_vec(Eigen::VectorXd a, Eigen::VectorXd b){
    if (a.size() != b.size()) {
        return false;
    }
    #if 0
    bool valid = true;
    for(int i = 0; i < a.size(); i++) {
        #ifdef HAVE_EIGEN
        valid = valid && eq_eps(a(i), b(i));
        #else
        valid = valid && eq_eps(a[i], b[i]);
        #endif
    }
    return valid;
    #elif defined(HAVE_REDI)
    #include <redi/zip.h>   // for a zip function which works with range-base for
                            // and accepts any number of ranges, which can be
                            // rvalues or lvalues and can be different lengths
                            // (iteration will stop at the end of the shortest range).

    void redi_zip() {
    std::vector<int> vi{ 0, 2, 4 };
    std::vector<std::string> vs{ "1", "3", "5", "7" };
    for (auto i : redi::zip(vi, vs))
        std::cout << i.get<0>() << ' ' << i.get<1>() << ' ';
    }
    #else
        for (auto tup : boost::combine(a, b, a)) {    // <---
            double x, y, w;
            boost::tie(x, y, w) = tup;
            printf("%g %g %g\n", x, y, w);
            if (! eq_eps(x, y)) {
                return false;
            }
        }
    //return internal::array_zip_and_reduce<eq_eps, a, b, a.size()>;  // same size
    #endif
    return true;
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
    double aa = 1.1, bb = 1.1, eps = 0.001;
    cout << "eq_eps(aa, bb, eps): " << eq_eps(aa,   bb,    eps) << endl;
    cout << "eq_eps(1.1, 1.1, 0.0001): " << eq_eps(1.1, 1.1, 0.0001) << endl;
    assert(eq_eps(1.1, 1.1, 0.0001));
    assert(! eq_eps(1.0001, 1.00025, 0.0001));
    cout << "eq_eps(  1, 1  ): " << eq_eps(1,   1          ) << endl;
    cout << "eq_eps(1.1, 1.1): " << eq_eps(1.1, 1.1) << endl;
    cout << "eq_eps(111, 111): " << eq_eps(111,111, 1) << endl;
    cout << "eq_eps(111, 112): " << eq_eps(111,112) << endl;
    cout << "eq_eps(111, 112, int(0.001)): " << eq_eps(111,112, int(0.001)) << endl;
    cout << "eq_eps(111, 112,  0): " << eq_eps(111,  112, 0) << endl;
    cout << "eq_eps(111, 113    ): " << eq_eps(111,  113   ) << endl;
    cout << "eq_eps(111, 113,  2): " << eq_eps(111,  113, 2) << endl;
    cout << "eq_eps(111, 114,  2): " << eq_eps(111,  113, 2) << endl;
    cout << "eq_eps(111, 5555  0): " << eq_eps(111, 5555, 0) << endl;
    cout << "eq_eps(111, 5555, 0): " << eq_eps(111, 5555, 0) << endl;
    return 0;
}
