// dirpoll.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 cli.cpp -o cli.out && ./cli.out
#if 1
#include <CLI11.hpp>
#else
#include <CLI/CLI.hpp>
#endif

#include <iostream>
using std::cerr;
using std::endl;

template<class T>
class A {
protected:
    A() { }
    T id = 37;
};

class B : private A<int> {
public:
    B() : A()
    {
        cerr << "B(): id: " << id << endl;
    }
};

int main(int argc, char **argv)
{

    B b;
    return 0;

    CLI::App app{"App description"};

    // Define options
    bool f;
    app.add_flag("-f", f, "flag f");

    int p = 0;
    app.add_option("-p", p, "int parameter");

    std::string s = "s";
    app.add_option("-s,--str", s, "string parameter");

    CLI11_PARSE(app, argc, argv);

    std::cout << "bool flag" " value: " << f << std::endl;
    std::cout << "int param value: " << p << std::endl;
    std::cout << "str param value: " << s << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "Examples of bad command(s):" << std::endl;
    std::cout << argv[0] << " -str=my_str" << std::endl;

    std::string ts("first part, " "second part.");
    std::cout << "TEST STRING: " <<  ts << std::endl;

    s = "";
    if (s.empty()) {
        std::cout << "nothing is true, and truth is empty!  (" << s << ")" << std::endl;
    } else {
        std::cout << "nothing is not only empty, it's also false!  (" << s << ")" << std::endl;
    }

    // throw std::runtime_error("one, " "two, " "three!");

    return 0;
}
