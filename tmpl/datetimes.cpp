// datetimes.cpp
// May require C11 or later
// BUILD: clang++ datetimes.cpp -o tmpl.out && tmpl.out

#include <chrono>
#include <iostream>
#include <time.h>

int main(int argc, char* argv[])    // NB: unit tests for MapTraj
{
    // std::cout << std::chrono::system_clock::now() << std::endl;
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // FIXME: call a member function: std::cout << "time_point: " << std::to_string(now) << std::endl;
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::cout << "time_point: " << std::to_string(now_c) << std::endl;
    std::tm now_tm = *std::localtime(&now_c);
    // TODO: individual parts: std::cout << "time_point: " << now_tm << std::endl;
    const size_t buf_size = 64;
    char cbuffer[buf_size];
    size_t n_chars = strftime(cbuffer, buf_size, "%Y.%m.%d_%H:%M:%S", &now_tm);
    std::string date_time(cbuffer, n_chars);
    std::cout << "Year.month.day_Hour:Minutes:Seconds from strftime (" << date_time << ") has length: "
              << n_chars << std::endl;
    std::cout << "For strftime docs, see: http://en.cppreference.com/w/c/chrono/strftime" << std::endl;
    std::cout << "For raw tm struct, see: http://en.cppreference.com/w/cpp/chrono/c/tm" << std::endl;
    return 0;
}
