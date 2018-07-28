// BUILD: clang++ -std=c++14 log_name.cpp -o tmpl && tmpl
#include "spdlog/spdlog.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

std::string make_name( const std::string& base, unsigned seq_num
                     , double xx, double yy, double zz
) {
    static const std::string sep("_");
    std::ostringstream oss;
    oss.precision(4);

    oss << base << sep << std::setfill('0') << std::setw(4) << seq_num
        << std::fixed << sep << abs(xx) << sep << abs(yy) << sep << abs(zz)
        << ".txt";
    return oss.str();
}


/// old-school way of getting the executable's full path in a *nix-like system.
char *get_program_full_path()
{
    char *path = (char *)malloc(PATH_MAX);
    if (path != NULL) {
        if (readlink("/proc/self/exe", path, PATH_MAX) == -1) {
            free(path);
            path = NULL;
        }
    }
    return path;
}

std::string get_program_short_name()
{
    std::string full_path = get_program_full_path();
    size_t dir_pos = full_path.find_last_of("/");
    std::string file_name = (dir_pos == std::string::npos) ? full_path : full_path.substr(dir_pos + 1);
    size_t dot_pos = file_name.find_last_of(".");
    std::string base_name = (dot_pos == std::string::npos) ? file_name : file_name.substr(0, dot_pos);
    return base_name;
}



int main(int, char* [])
{
    auto console = spdlog::stdout_color_st("console");
    console->info("Welcome to spdlog!") ;
    auto name = make_name("name", 23, -0.1234, 0.5678, -0.9012);
    console->info("The name: <{}>", name);
    const char *full_path = get_program_full_path();
    console->info("The program's full path, from proc: {}", (full_path == nullptr ? "NULL" : full_path));
}
