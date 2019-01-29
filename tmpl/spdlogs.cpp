// spdlogs.cpp
// May require C11 or later
// BUILD:     g++ -std=c++11 spdlogs.cpp -o tmpl.out && tmpl.out
// BUILD: clang++ -std=c++11 spdlogs.cpp -I ~/asdf/spdlog/include -o tmpl.out && tmpl.out

#include "../../spdlog/include/spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <exception>    // For std::exception
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

std::string getenv_var(const std::string& var) {
     const char *val = std::getenv(var.c_str());
     if (val == 0) {
         return "";
     } else {
         return val;    // auto-converted
     }
}

bool IsEnvCircleCi() { return getenv_var("CIRCLECI") == "true"; }

#define USE_PROMPT_USER 1
#define BUFSIZE 128
void prompt_user(const std::string& prompt)
{
#if USE_PROMPT_USER
    if (IsEnvCircleCi()) {
        cout << "IsEnvCircleCi() => true" << endl;
        return;
    }
    // std::string input;
    std::cout << prompt << "  To continue, press enter... " << std::endl;
    char line[BUFSIZE];
    if (fgets(line, BUFSIZE, stdin) != 0)
    {
        line[strcspn(line, "\n")] = '\0';
        std::cout << "Got <" << line << ">, continuing..." << std::endl;
    }
    // std::cin >> input;
#endif
}


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

    // Multithreaded console logger
    std::shared_ptr<spdlog::logger> console = spdlog::stdout_logger_mt("console");
    console->info("Welcome to spdlog!") ;

    auto name = make_xyz_name("name", 23, -0.1234, 0.5678, -0.9012);
    console->info("The xyz-name: <{}>", name);

    console->info("name.c_str(): <{}>", name.c_str());  // 2nd argument is lost
    console->info("(                )", name.c_str());
    console->info(name.c_str());
    console->info(name);

    console->info("3-wide int({:3})", 2);
    console->info("3-wide int({:3})", 11);


    prompt_user("hahahaha A");
    prompt_user("hahahaha B");
    prompt_user("hahahaha C");
    prompt_user("hahahaha E");
    prompt_user("hahahaha E");
    return -14;

    try
    {
        // Create basic file logger (not rotated)
        auto basic_logger = spdlog::basic_logger_mt("basic", log_dir_path + "/basic.txt");
        basic_logger->info("Hello {} {} !!", "param1", 123.4);

        // create a file rotating logger with 5mb size max and 3 rotated files
        auto rfile_logger = spdlog::rotating_logger_mt("rfile", log_dir_path + "/rotated.txt", 1024 * 1024 * 5, 3);
        rfile_logger->info("Howdy {} {} !!", "param1", 4.5678);
        int n = 42;
        printf(            "printf:    n == %d \t &n == %p\n", n, &n);
        std::cout <<       "std::cout: n == " << n << " \t " << "&n == " << std::hex << long(&n) << std::endl;
        basic_logger->warn("basic_log: n == {} \t &n == {:x}", n, long(&n));
        console->info("Formt string width: [12345678901234567890]");
        console->info("Formt string width: [{:20}]", "string");
        console->info("Formt string width: [{:>20}]", "string");
        console->info("Formt string width: [{:>{}}]", "string", 20);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}
