// remove_dir_rec.cpp.cpp
// May require C11 or later
// BUILD: clang++ backup_to_num.cpp -o tmpl.out && tmpl.out
// BUILD:     g++ backup_to_num.cpp -o tmpl.out && tmpl.out

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


/// call rmdir or unlink on the path, depending on the flag
int rm_dir_or_file(const char *path, const struct stat *pStat, int flag, struct FTW *pFtw)
{
    int status = 0;
    if (flag == FTW_DP) {
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

bool split_file_path( const std::string& file_path
                    , std::string& beg
                    , std::string& end
                    , const std::string& str = "."
) {
    size_t pos = file_path.find_last_of(str);
    if (pos != std::string::npos) {
        beg = file_path.substr(0, pos);
        end = file_path.substr(pos);    // includes str
        return true;
    }
    beg = file_path;
    end = "";
    return false;
}

int rename_file(const std::string& old_name, const std::string& new_name)
{
    int result = rename(old_name.c_str(), new_name.c_str());
    if (result != 0) {
        char *error = errno ? strerror(errno) : strerror(result);
        cerr<< "Error from rename_file(" << old_name << ", " << new_name << "): "
            << error << endl;
        return result;
    }
    return 0;
}


/// Renames an existing file named like file.ext into a series numbered back ups,
/// as follows:
/// file.<N-1>.ext => file.<N>.ext for N = max_numbered_backup_files down to 1;
/// file.ext => file.1.ext
/// If path does not include a file extension, then path => path.1, etc.
/// Returns the number of numbered backup files left behind,
/// which is 0 if path identifies a directory or anything but a rename-able file.
int backup_to_num(const char *path, int max_numbered_backup_files = 9)
{
    int err, num_renamed = 0;
    if (nullptr == path || 0 == strlen(path)) {
        cerr << "backup_to_num: empty file name" << endl;
        return -1;
    }
    struct stat result;
    if (0 != (err = stat(path, &result))) {
        perror(path);
        return -2;
    }
    if (result.st_mode & S_IFDIR) {
        cerr << "backup_to_num: path (" << path << ") is a directory" << endl;
        return -3;
    }
    std::string base, extn, backup_file;
    split_file_path(path, base, extn);
    std::string backup_plus_1 = base + "." + std::to_string(max_numbered_backup_files)
                              + extn;
    for (int j = max_numbered_backup_files; --j > 0; ) {
        backup_file = base + "." + std::to_string(j) + extn;
        if (0 == stat(backup_file.c_str(), &result)) {
            cerr << "Moving " << backup_file << " to " << backup_plus_1 << endl;
            if (0 != rename_file(backup_file, backup_plus_1)) {
                return -4 - j;
            }
            num_renamed++;
        }
        backup_plus_1 = backup_file;
    }
    cerr << "Moving " << path << " to " << backup_file << endl;
    if (0 != rename_file(path, backup_file)) {
        return -5 - max_numbered_backup_files;
    }
    return 1 + num_renamed;
}

int main(int argc, char* argv[])    // NB: unit tests for MapTraj
{
    const char *path = argc > 1 ? argv[1] : "derp.out";
    cerr << argv[0] << " " << (path ? path : "NULL")  << endl;
    backup_to_num(path);

    int status = rename_file("a.out", "b.out");
    int statut = rename_file("b.out", "a.out");
    cerr << "status = rename_file(\"a.out\", \"b.out\") ==> " << status << endl;
    cerr << "statut = rename_file(\"b.out\", \"a.out\") ==> " << status << endl;
    return 0;
}
