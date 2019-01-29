// datetimes.cpp
// May require C11 or later
// BUILD: clang++ datetimes.cpp -o tmpl.out && tmpl.out

#include <chrono>
#include <locale>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <time.h>

static const char *TS_FORMAT = "%Y%m%dT%H%M%S";

std::string get_datetime_string()
{
    std::time_t time = std::time(nullptr);
    std::tm* tm = std::localtime(&time);

    std::ostringstream ss, ss2;
    ss << std::put_time(tm, TS_FORMAT);

    std::string date_string = ss.str();
    return date_string;
}

void std_get_time_a(const std::string& ts = "20181103T125832")
{
    std::tm t = {};
    std::istringstream ss(ts);
    ss >> std::get_time(&t, TS_FORMAT);
    std::cout << "std_get_time_a: " << ts << ": ";
    if (ss.fail()) {
        std::cout << "std_get_time_a: Parse failed" << std::endl;
    } else {
        time_t ts_seconds = std::mktime(&t);
        std::cout << std::put_time(&t, "%c")
        << "  ts_seconds: " << ts_seconds
        << std::endl;
    }
}


void std_get_time_b(const std::string& ts = "2010-11-04T23:23:01Z")
{
    std::tm t = {};
    std::istringstream ss(ts);

    std::cout << "std_get_time_b: " << ts << ": ";
    if (ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
    {
        std::cout << std::put_time(&t, "%c") << "  ts_seconds: "
                  << std::mktime(&t) << "\n";
    }
    else
    {
        std::cout << "Parse failed\n";
    }
}

void std_get_time_c(const std::string& ts = "2011-Februar-18 23:12:34")
{
    std::tm t = {};
    std::istringstream ss(ts);
    ss.imbue(std::locale("de_DE.utf-8"));
    ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
    std::cout << "std_get_time_c: " << ts << ": ";
    if (ss.fail()) {
        std::cout << "Parse failed\n";
    } else {
        std::cout << std::put_time(&t, "%c") << '\n';
    }
}

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
    std::string ts_string = get_datetime_string();
    std_get_time_a(ts_string);
    std_get_time_b();
    std_get_time_c();
    return 0;
}
