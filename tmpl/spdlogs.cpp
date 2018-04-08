#include "spdlog/spdlog.h"
#include <exception>    // For std::exception
#include <iostream>
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



int main(int, char* [])
{
    std::string log_dir_path("logs");
    make_dir(log_dir_path);

    //Multithreaded console logger(with color support)
    auto console = spdlog::stdout_color_mt("console");
    console->info("Welcome to spdlog!") ;
    console->info("An info message example {}..", 1);

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
