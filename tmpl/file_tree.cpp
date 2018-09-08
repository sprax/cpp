// remove_dir_rec.cpp.cpp
// May require C11 or later
// BUILD: clang++ -std=c++11 file_tree.cpp -o tmpl.out && tmpl.out
// BUILD:     g++ -std=c++11 file_tree.cpp -o tmpl.out && tmpl.out

// #include <dirent.h>
#include <ftw.h>
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                 // rmdir & unlink

#ifndef OPEN_MAX
#define OPEN_MAX 0                  // Guess 256 if it must be determinate
#endif

using std::cerr;
using std::endl;

#include <ftw.h>
#include <iostream>

using namespace std;

int two_arg_fn(const char *p1, const char *p2)
{
    printf("p1(%s) AND p2(%s)\n", p1, p2);
    return 0;
}



int main()
{
    auto callback = [](const char *fpath, const struct stat *sb,
        int typeflag) -> int {
        return two_arg_fn(fpath, (string("hahahaha/") + fpath).c_str());
    };

    int ret = ftw("doh.log", callback, 1);

    return ret;
}
// int main(int argc, char *argv[])    // NB: unit tests for MapTraj
// {
//     // const auto root = fs::current_path();
//     // cerr << "root = fs::current_path(); // => " << root << endl;
//     // return 0;
//
//     const char *src_dir = argc > 1 ? argv[1] : "doh.sprax";
//     const char *dst_dir = argc > 2 ? argv[1] : "duh.sprax";
//     cerr << argv[0] << " " << src_dir << " " << dst_dir << endl;
//     copy_dir_rec(src_dir, dst_dir);
//
//
//     // std::filesystem::copy("/dir1", "/dir3", std::filesystem::copy_options::recursive);
//
//
//     return 0;
// }
