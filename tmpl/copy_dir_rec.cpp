// remove_dir_rec.cpp.cpp
// May require C11 or later
// BUILD: clang++ copy_dir_rec.cpp -o tmpl.out && tmpl.out
// BUILD:     g++ copy_dir_rec.cpp -o tmpl.out && tmpl.out

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


/// call rmdir or unlink on the path, depending on the ftw_flag
int rm_dir_or_file(const char *path, const struct stat *pStat, int ftw_flag, struct FTW *pFtw)
{
    int status = 0;
    if (ftw_flag == FTW_DP) {
        status = rmdir(path);
        cerr << "status = rmdir(" << path << "); // => " << status << endl;
    } else {
        status = unlink(path);
        cerr << "status = unlink(" << path << "); // => " << status << endl;
    }
    if (status != 0) {
        perror(path);
    } else {
        cerr << "removed: " << path << endl;
        // cerr << "visited: " << path << endl;
    }
    return status;
}


/// implements system("rm -f -R dir_spec")
int remove_dir_rec(const char *dir_spec)
{
    if (nftw(dir_spec, rm_dir_or_file, OPEN_MAX, FTW_DEPTH)) {
        perror(dir_spec);
        return 1;
    }
    return 0;
}


int copy_dir_or_file( const char *src_path
                    , const char *dst_path      //
                    , const struct stat* sb
                    , int ftw_flag
) {
    std::string dst_path = dst_root + src_path;
    switch(ftw_flag) {
    case FTW_D:
        return mkdir(dst_path.c_str(), sb->st_mode);
    case FTW_F:
        std::ifstream src(src_path, std::ios::binary);
        std::ofstream dst(dst_path, std::ios::binary);
        dst << src.rdbuf();
        return 0;               // TODO: check status?
    default:
        cerr << "copy_dir_or_file: ftw_flag = " << ftw_flag << " is neither FTW_D nor FTW_F" << endl;
        return 1;
    }
    return 0;
}

/// implements system("copy -f -R dir_spec")
int copy_dir_rec(const char *src_dir, const char *dst_dir)
{
    if (! src_dir || ! dst_dir) {
        return 2;
    }
    src_len = strlen(src_dir);
    std::string dir_spec = src_dir ? src_dir
    if (nftw(dir_spec, copy_dir_or_file, OPEN_MAX, FTW_DEPTH)) {
        perror(dir_spec);
        return 1;
    }
    return 0;
}


int main(int argc, char *argv[])    // NB: unit tests for MapTraj
{
    const auto root = fs::current_path();
    cerr << "root = fs::current_path(); // => " << root << endl;
    return 0;

    const char *src_dir = argc > 1 ? argv[1] : "doh.sprax";
    const char *dst_dir = argc > 2 ? argv[1] : "duh.sprax";
    cerr << argv[0] << " " << src_dir << " " << dst_dir << endl;
    copy_dir_rec(src_dir, dst_dir);


    // std::filesystem::copy("/dir1", "/dir3", std::filesystem::copy_options::recursive);


    return 0;
}
