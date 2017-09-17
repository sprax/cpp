// Compile and Run:
// Mac osx:
//  GCC g++ vs. CLANG clang++ (both supply C++ library linkage):
//      g++ hello.cpp -o hello.out && hello.out  [you might need ./hello.out]
//      clang++ hello.cpp -o hello.out && hello.out  [you might need ./hello.out]
//  GCC and CLANG front ends, with explicit C++ library link options: 
//      gcc -lc++ hello.cpp -o hello.out && hello.out
//      clang -lc++ hello.cpp -o hello.out && hello.out (default C++ library)
//      clang -lstdc++ hello.cpp -o hello.out && hello.out (Use libstdc++ for compatibility with libMLi3)
//  CC is a symbolic link to clang:  /usr/bin/CC@ -> clang  
//
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    // Using std string plus operator:  std::string + const char * = std::string
    const string usage = string(" [usage: ") + argv[0] + " name]";
    const string name = argc > 1 ? argv[1] : string("Nameless One") + usage;
    cout << "This is " << argv[0] << ", just saying Hello, " << name << "."  << endl;
    return 0;
}

