// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 traj_map_.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 traj_map_.cpp -o tmpl && tmpl

#include <chrono>
#include <glob.h>
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
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;


inline bool glob(const std::string& pat, std::vector<std::string>& result)
{
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    result = vector<string>();
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        result.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return result.size() > 0;
}

static std::vector<std::string> keys{"keyA", "keyB", "keyC", "keyD"};

int main(int argc, char* argv[])
{
    string pattern = "*.cpp";
    std::vector<std::string> files;
    if (glob(pattern, files)) {
        int j = 0;
        for (auto& key : files) {
            cout << "key: " << key << endl;
            if (--j == 0) {
                break;
            }
        }
    } else {
        cout << "No files found for: " << pattern << endl;
    }

    return 0;
}
