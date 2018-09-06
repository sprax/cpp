// remove_dir_rec.cpp.cpp
// May require C11 or later
// BUILD: clang++ remove_dir_rec.cpp -o tmpl && tmpl

#include <ftw.h>
#include <iostream>
// #include <stdlib.h>
// #include <stdio.h>
// #include <sys/syslimits.h>
#include <unistd.h>                 // rmdir & unlink

using std::cerr;
using std::endl;


/// call rmdir or unlink on the path, depending on the flag
int rm_dir_or_file(const char *path, const struct stat *pStat, int flag, struct FTW *pFtw)
{
    int status = 0;
    if (flag == FTW_DP) {
        status = rmdir(path);
        cerr << "status = rmdir(" << path << "); => " << status << endl;
    } else {
        status = unlink(path);
        cerr << "status = unlink(" << path << "); => " << status << endl;
    }
    if (status != 0) {
        perror(path);
    } else {
        cerr << "removed: " << path << endl;
        // cerr << "visited: " << path << endl;
    }
    return status;
}


/// implements system("rm -rf")
int remove_dir_rec(const char *dir_name)
{
    if (nftw(dir_name, rm_dir_or_file, OPEN_MAX, FTW_DEPTH)) {
        perror(dir_name);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int main(int argc, char* argv[])    // NB: unit tests for MapTraj
{
    const char *the_dir = argc > 1 ? argv[1] : "duh";
    cerr << argv[0] << " " << the_dir << endl;
    remove_dir_rec(the_dir);
    return 0;
}
