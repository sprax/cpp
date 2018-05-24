// drake_logs.cpp : use drake text_logging
// Requires C11 or later and drake binaries installed under /opt
// To build and run:
// export LD_LIBRARY_PATH=/opt/drake/lib/:$LD_LIBRARY_PATH
// clang++ -std=c++11 -DCMAKE_BUILD_TYPE=Debug -I/opt/drake/include drake_logs.cpp -lgflags /opt/drake/lib/libdrake.so -o log.out && log.out

#include <exception>    // For std::exception
#include <iostream>
#include <sys/stat.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "/opt/drake/include/drake/common/text_logging_gflags.h"
#include "spdlog/spdlog.h"

void make_dir(const std::string& path)
{
    if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        if (errno == EEXIST) {
           // already exists
        } else {
           // something else
            std::cout << "cannot create error:" << strerror(errno) << std::endl;
            throw std::runtime_error( strerror(errno) );
        }
    }
}


int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    drake::logging::HandleSpdlogGflags();

    std::string log_dir_path("logs");
    make_dir(log_dir_path);

    // Multithreaded console logger(with color support)
    // auto console = spdlog::stdout_color_mt("console");
    // console->info("Welcome to spdlog!") ;
    // console->info("An info message example {}..", 1);

    std::cout << "Trying drake::log()->info(...)" << std::endl;
    std::vector<double> std_vec = {1.1, 2.2, 3.3};
    drake::log()->info("Testing drake::logger as if from the robot's config ... [1]");
    drake::log()->info("Testing drake::logger as if from the robot's config ... [2]");
    drake::log()->info("Test A: literal int {}", 42);
    drake::log()->warn("Test B: comma sep strings: {}", " interpolated string");
    SPDLOG_DEBUG(drake::log(), "SPDLOG_DEBUG with braces: {}", "JUST TESTING");
#if 0
    auto logger = drake::log();
    const std::vector<spdlog::sink_ptr> my_sinks = logger->sinks();

    for (auto const& value : std_vec) {
        drake::log()->info("Test C: std::vector: {}", value);
    }
    for (auto const& sink : my_sinks) {
        drake::log()->info("Test D: sinks: {}", sink);
    }
#endif

    // try
    // {
    //     // Create basic file logger (not rotated)
    //     auto basic_logger = spdlog::basic_logger_mt("basic", log_dir_path + "/basic.txt");
    //     basic_logger->info("Hello {} {} !!", "param1", 123.4);
    //
    //     // create a file rotating logger with 5mb size max and 3 rotated files
    //     auto rfile_logger = spdlog::rotating_logger_mt("rfile", log_dir_path + "/rotated.txt", 1024 * 1024 * 5, 3);
    //     rfile_logger->info("Howdy {} {} !!", "param1", 4.5678);
    // }
    // catch (const spdlog::spdlog_ex& ex)
    // {
    //     std::cout << "Log initialization failed: " << ex.what() << std::endl;
    //
    return 0;
}
