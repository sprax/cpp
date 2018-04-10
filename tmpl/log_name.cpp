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

int main(int, char* [])
{
    auto console = spdlog::stdout_color_st("console");
    console->info("Welcome to spdlog!") ;
    auto name = make_name("name", 23, -0.1234, 0.5678, -0.9012);
    console->info("The name: <{}>", name);
}
