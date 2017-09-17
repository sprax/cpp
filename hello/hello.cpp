// Compile and Run:
// Mac osx: g++ hello.cpp -o hello.out && hello.out  [you might need ./hello.out]
//
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
  const string usage = string(" [usage: ") + argv[0] + " name]";
  const string name = argc > 1 ? argv[1] : string("Nameless One") + usage;
  cout << "This is " << argv[0] << ", just saying Hello, " << name << "."  << endl;
  return 0;
}

