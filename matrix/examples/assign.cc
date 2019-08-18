/// Compile and Run:
/// Mac osx:
/*  CLANG clang++ vs. GCC g++ (both supply C++ library linkage):
        clang++ -std=c++17 matrices.cpp -o tmp.out && tmp.out  	# [you might need ./tmp.out]
            g++ -std=c++17 matrices.cpp -o tmp.out && tmp.out  	# [you might need ./tmp.out]
    GCC and CLANG front ends, with explicit C++ library link options:
        gcc -std=c++17 -lc++ matrices.cpp -o tmp.out && tmp.out
        clang -lc++ matrices.cpp -o tmp.out && tmp.out (default C++ library)
        clang -std=c++11 -lstdc++ matrices.cpp -o tmp.out && tmp.out #(Use libstdc++ for compatibility with libMLi3)
    CC is a symbolic link to clang:  /usr/bin/CC@ -> clang
   Windows 10: very similar:
        g++  -std=c++11 matrices.cpp -o matrices.exe && matrices
*/
#include "matrix/matrix.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    cout << argv[0] << ":" << endl;

    VVMatrix<double> vvmA(2, 4, 5.0);
    cout << "vvmA: " << vvmA.getRows() << " x " << vvmA.getCols() << " value: "
         << vvmA(0, 0) << " == " << vvmA[0][0] << endl;
    VVMatrix<double> vvmB(3, 5, 6.0);
    cout << "Reassigning: vvmA = vvmB:" << endl;
    vvmA = vvmB;
    cout << "vvmA: " << vvmA.getRows() << " x " << vvmA.getCols() << " value: "
         << vvmA(0, 0) << " == " << vvmA[0][0] << endl;

    std::vector<double> vec = {1.1, 2.2, 3.3};
    std::cout << "vec * 1: " << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
    for (double& dd : vec) {
         dd *= 2;
    }
    std::cout << "vec * 2: " << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
    return 0;
}
