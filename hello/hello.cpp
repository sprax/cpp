// Compile and Run:
// Mac osx: g++ hello.cpp -o hello.out && ./hello.out
//
#include <iostream>
//#include <string>
using namespace std;

int main(int argc, char* argv[]) {
  const char *name = argc > 1 ? argv[1] : "Nameless One";
  cout << "This is " << argv[0] << ", just saying Hello, " << name << "."  << endl;
  return 0;
}

