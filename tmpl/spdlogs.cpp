#include "spdlog/spdlog.h"
#include <exception>    // For std::exception
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>

void make_dir(const std::string& path)
{
    if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if( errno == EEXIST ) {
           // alredy exists
        } else {
           // something else
            std::cout << "cannot create error:" << strerror(errno) << std::endl;
            throw std::runtime_error( strerror(errno) );
        }
    }
}

std::string make_xyz_name( const std::string& base, unsigned seq_num
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
    std::string log_dir_path("logs");
    make_dir(log_dir_path);

    //Multithreaded console logger(with color support)
    auto console = spdlog::stdout_color_mt("console");
    console->info("Welcome to spdlog!") ;

    auto name = make_xyz_name("name", 23, -0.1234, 0.5678, -0.9012);
    console->info("The xyz-name: <{}>", name);

    console->info("name.c_str(): <{}>", name.c_str());  // 2nd argument is lost
    console->info("(                )", name.c_str());
    console->info(name.c_str());
    console->info(name);

    try
    {
        // Create basic file logger (not rotated)
        auto basic_logger = spdlog::basic_logger_mt("basic", log_dir_path + "/basic.txt");
        basic_logger->info("Hello {} {} !!", "param1", 123.4);

        // create a file rotating logger with 5mb size max and 3 rotated files
        auto rfile_logger = spdlog::rotating_logger_mt("rfile", log_dir_path + "/rotated.txt", 1024 * 1024 * 5, 3);
        rfile_logger->info("Howdy {} {} !!", "param1", 4.5678);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
