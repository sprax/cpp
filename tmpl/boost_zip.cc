#include <boost/range/combine.hpp>
#include <vector>
#include <list>
#include <string>

#ifdef HAVE_REDI
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
#endif

int main() {
    std::vector<int> a {4, 5, 6};
    double b[] = {7, 8, 9};
    std::list<std::string> c {"a", "b", "c"};
    for (auto tup : boost::combine(a, b, c, a)) {    // <---
        int x, w;
        double y;
        std::string z;
        boost::tie(x, y, z, w) = tup;
        printf("%d %g %s %d\n", x, y, z.c_str(), w);
    }
    #ifdef HAVE_REDI
    redi_zip();
    #endif  // HAVE_REDI
}
