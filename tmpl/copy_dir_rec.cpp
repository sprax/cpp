// remove_dir_rec.cpp.cpp
// May require C11 or later
// BUILD: clang++ -std=c++11 copy_dir_rec.cpp -o tmpl.out && tmpl.out
// BUILD:     g++ -std=c++11 copy_dir_rec.cpp -o tmpl.out && tmpl.out

// #include <dirent.h>
#include <ftw.h>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                 // rmdir & unlink
#include <functional>

using namespace std;




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
                    , const std::string& dst_path
                    , const struct stat* sb
                    , int ftw_flag
) {

    std::string dst_full = dst_path + "/" + src_path;
    printf("copy_dir_or_file(%s, %s, pstat, %d)\n", src_path, dst_full.c_str(), ftw_flag);

    switch(ftw_flag) {
    case FTW_D:
        return mkdir(dst_full.c_str(), sb->st_mode);
    case FTW_F: {
            std::ifstream src(src_path, std::ios::binary);
            std::ofstream dst(dst_full, std::ios::binary);
            dst << src.rdbuf();
        }
        return 0;               // TODO: check status?
    // default:
    //     cerr << "copy_dir_or_file: ftw_flag = " << ftw_flag << " is neither FTW_D nor FTW_F" << endl;
    //     return 1;
    }
    return 0;
}

/// functor as C-function ?
class Append {
private:
    const std::string dst_;
public:
    Append(const char *dst) : dst_(dst)
    { }
    std::string operator () (const char *src) { return dst_ + " | " + src; }
};



namespace {
   thread_local size_t gTotalBytes{0};  // thread local makes this thread safe
int GetSize(const char* path, const struct stat* statPtr, int currentFlag, struct FTW* internalFtwUsage) {
    gTotalBytes +=  statPtr->st_size;
    cout << path << "  total: " << gTotalBytes << endl;
    return 0;  //ntfw continues
 }
} // namespace


namespace {
   thread_local std::string gDstDir("FLOOZLE: ");  // thread local makes this thread safe
int CopyToDstDir(const char* path, const struct stat* statPtr, int ftw_flag, struct FTW*) {
    return copy_dir_or_file(path, gDstDir.c_str(), statPtr, ftw_flag);
    // return 0;  //ntfw continues
 }
} // namespace


/// implements system("copy -f -R dir_spec")
int copy_dir_rec(const char *src_dir, const char *dst_dir)
{
    if (! src_dir || ! dst_dir) {
        return 2;
    }
    // size_t dst_len = strlen(dst_dir);

    // // cannot implicitly (or otherwise) convert any capturing lambda
    // // directly to C-function pointer.
    // auto copy_func = [&](const char *src_path, const struct stat *sb,
    //     int ftw_flag, struct FTW *ftw_struct) -> int {
    //     return copy_dir_or_file(src_path, dst_dir, sb, ftw_flag);
    // };

    // // Can convert reference-capturing lambda to std::function,
    // // but cannot convert std::function to C-function pointer.
    // std::function< int ( const char *src_path, const struct stat *sb
    //                    , int ftw_flag, struct FTW *ftw_struct )>
    //     copy_func = [&]( const char *src_path, const struct stat *sb
    //                    , int ftw_flag, struct FTW *ftw_struct) -> int {
    //         return copy_dir_or_file(src_path, dst_dir, sb, ftw_flag);
    // };

    Append append(dst_dir);
    cerr << "append: " << append(src_dir) << endl;

    // NOT a copy function!
    auto copy_func = [](const char *src_path, const struct stat *sb,
        int ftw_flag, struct FTW *ftw_struct) -> int {
            cerr << "////" << src_path << endl;
            return 0;
    };

    gDstDir = dst_dir;
    // std::string dir_spec = src_dir ? src_dir
    mkdir(dst_dir, S_IRWXU);
    if (nftw(src_dir, CopyToDstDir, OPEN_MAX, FTW_DEPTH)) {
        perror(src_dir);
        return 1;
    }
    return 0;
}

// #include <experimental/filesystem>
// #include <filesystem>
// namespace fs = std::experimental::filesystem; // In C++17 use std::filesystem

int main(int argc, char *argv[])    // NB: unit tests for MapTraj
{
    // const auto root = fs::current_path();
    // cerr << "root = fs::current_path(); // => " << root << endl;
    // return 0;

    const char *src_dir = argc > 1 ? argv[1] : "doh.log";
    const char *dst_dir = argc > 2 ? argv[1] : "DUH.log";
    cerr << argv[0] << " " << src_dir << " " << dst_dir << endl;
    copy_dir_rec(src_dir, dst_dir);


    // std::filesystem::copy("/dir1", "/dir3", std::filesystem::copy_options::recursive);


    return 0;
}
