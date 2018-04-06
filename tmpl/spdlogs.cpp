#include <iostream>
#include "spdlog/spdlog.h"

int main(int, char* [])
{
    //Multithreaded console logger(with color support)
    auto console = spdlog::stdout_color_mt("console");
    console->info("Welcome to spdlog!") ;
    console->info("An info message example {}..", 1);

    try
    {
        // Create basic file logger (not rotated)
        auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
        my_logger->info("Hello {} {} !!", "param1", 123.4);

        // create a file rotating logger with 5mb size max and 3 rotated files
        auto file_logger = spdlog::rotating_logger_mt("file_logger", "myfilename", 1024 * 1024 * 5, 3);
        file_logger->info("Howdy {} {} !!", "param1", 4.5678);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
