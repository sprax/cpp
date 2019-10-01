// remove_dir_rec.cpp.cpp
// May require C11 or later
/** BUILD:
clang++ -std=c++17 -I ../externals/filesystem/include/ file_tree.cpp -o tmpl.out && tmpl.out
    g++ -std=c++11 file_tree.cpp -o tmpl.out && tmpl.out
**/

// #include <dirent.h>
#if defined(__cplusplus) && __cplusplus >= 201703L && defined(__has_include) && __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <ghc/filesystem.hpp>
namespace fs = ghc::filesystem;
#endif

#include <ftw.h>
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                 // rmdir & unlink

#ifndef OPEN_MAX
#define OPEN_MAX 0                  // Guess 256 if it must be determinate
#endif

using namespace std;

#include <ftw.h>
#include <iostream>

using namespace std;

int two_arg_fn(const char *p1, const char *p2)
{
    printf("p1(%s) AND p2(%s)\n", p1, p2);
    return 0;
}


int fpath_lambda()
{
    auto callback = [](const char *fpath, const struct stat *sb,
        int typeflag) -> int {
        return two_arg_fn(fpath, (string("hahahaha/") + fpath).c_str());
    };

    int ret = ftw("doh.log", callback, 1);

    return ret;
}

int main(int argc, char *argv[])    // NB: unit tests for MapTraj
{
    const auto root = fs::current_path();
    cerr << "root = fs::current_path(); // => " << root << endl;

    for(fs::path p : {"/var/tmp/example.txt", "/", "/var/tmp/.", "haha"}) {
        cout<< "The parent path of " << p
            << " is " << p.parent_path() << endl;
        cout<< "The parent path file name of " << p
            << " is " << p.parent_path().filename() << endl;
    }

    return 0;
}
