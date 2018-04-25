// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 traj_map_.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 traj_map_.cpp -o tmpl && tmpl

#include <chrono>
#include <glob.h>
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;
using std::string;

time_t get_file_ctime(char *path)
{
    struct stat attr;
    stat(path, &attr);
    // printf("Last modified time: %s", ctime(&attr.st_ctime));
    return attr.st_ctime;
}

time_t get_file_mtime(const char *path)
{
    struct stat attr;
    stat(path, &attr);
    //printf("Last modified time: %s", ctime(&attr.st_mtime));
    return attr.st_mtime;
}

char * get_latest_file_name(const char *dir_path, const char *extension)
{
    char * latest_file_name = NULL;
    time_t latest_file_time = 0;
    DIR           *pDIR;
    struct dirent *pDirent;
    pDIR = opendir(dir_path);
    if (pDIR) {
        while ((pDirent = readdir(pDIR)) != NULL)
        {
            //exclude "." and ".."
            if (strcmp( pDirent->d_name, "." ) == 0 ||
            strcmp( pDirent->d_name, ".." ) == 0 )
            {
                continue;
            }
            char *file_name = pDirent->d_name;
            printf("%s\n", file_name);
            time_t file_time = get_file_mtime(file_name);
            if (latest_file_time < file_time) {
                latest_file_time = file_time;
                latest_file_name = file_name;
            }
        }
        closedir(pDIR);
    }
    return latest_file_name;
}

inline bool glob_b(const std::string& pat, std::vector<std::string>& result)
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

inline std::vector<std::string> glob_v(const std::string& pat)
{
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    vector<string> ret;
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        ret.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

static std::vector<std::string> keys{"keyA", "keyB", "keyC", "keyD"};

std::vector<std::string> get_cpp_files(int max_num_files)
{
    string pattern = "*.cpp";
    std::vector<std::string> files;
    if (glob_b(pattern, files)) {
        int j = max_num_files;
        for (auto& file : files) {
            cout << "file: " << file << endl;
            if (--j == 0) {
                break;
            }
        }
    } else {
        cout << "No files found for: " << pattern << endl;
    }
    return files;
}

int main(int argc, char* argv[])
{
    int max_num_files = 0;
    std::vector<std::string> cpp_files = get_cpp_files(max_num_files);
    return 0;
}
