// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 traj_map_.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 traj_map_.cpp -o tmpl && tmpl

/// TODO: use full_path or rel_path, not .
#include <dirent.h>
#include <chrono>
#include <cstring>
#include <glob.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// using std::cin;
// using std::cout;
// using std::endl;
// using std::string;

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

std::string get_file_extension(std::string file_path)
{
    // Finds the last persiod character of the std::string
    int dot_pos = file_path.find_last_of(".");
    std::string ext = file_path.substr(dot_pos + 1);
    return ext;
}

char * get_latest_file_name(const char *dir_path, const char *extension, int verbose=1)
{
    char * latest_file_name = NULL;
    time_t latest_file_time = 0;
    DIR           *pDIR;
    struct dirent *pDirent;
    pDIR = opendir(dir_path);
    if (pDIR) {
        while ((pDirent = readdir(pDIR)) != NULL)
        {
            char *file_name = pDirent->d_name;
            //exclude "." and ".."
            if (strcmp(file_name, "." ) == 0 || strcmp(file_name, "..") == 0) {
                continue;
            }
            if (verbose > 1) {
                printf("GLFN ANY: %s\n", file_name);
            }
            std::string file_ext = get_file_extension(file_name);
            if (file_ext != extension) {
                continue;
            }
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
    result = vector<std::string>();
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        result.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return result.size() > 0;
}

inline std::vector<std::string> glob_v(const std::string& pat)
{
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    vector<std::string> ret;
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        ret.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

static std::vector<std::string> keys{"keyA", "keyB", "keyC", "keyD"};

std::vector<std::string> get_cpp_files(int max_num_files)
{
    std::string pattern = "*.cpp";
    std::vector<std::string> files;
    if (glob_b(pattern, files)) {
        int j = max_num_files;
        for (auto& file : files) {
            std::cout << "file: " << file << std::endl;
            if (--j == 0) {
                break;
            }
        }
    } else {
        std::cout << "No files found for: " << pattern << std::endl;
    }
    return files;
}

int main(int argc, char* argv[])
{
    const char *dir_path = argc > 1 ? argv[1] : "/Users/sprax/asdf/cpp";
    int verbose = argc > 2 ? atoi(argv[2]) : 1;

    std::string sepl = "--------------------------\n";
    int max_num_files = 0;
    std::vector<std::string> cpp_files = get_cpp_files(max_num_files);
    std::cout << sepl;
    const char *extension = "cpp";
    char *latest_file_name = get_latest_file_name(dir_path, extension, verbose);
    std::string fname(latest_file_name ? latest_file_name : "[NONE]");
    std::cout << sepl << "latest_file_name: " << fname << std::endl;
    return 0;
}
