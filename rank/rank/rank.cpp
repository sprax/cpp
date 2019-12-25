// rank.cpp : Defines the main entry point for the console application.
// rank?  All this does is time a sleep utility.

/** Example build commands:
clang++ -std=c++11 rank.cpp -o tmp.out && tmp.out
    g++ -std=c++14 rank.cpp -o tmp.out && tmp.out
*/
#ifdef WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include <chrono>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#define SLEEP_MS(A) Sleep(A)
#else
#define SLEEP_MS(A) usleep(A * 1000) //  Linux and Mac OS X
// Could also use boost::this_thread::sleep() if already using Boost
#endif

using namespace std;

int main(int argc, char *argv[])
{
    unsigned long millis = 3141;
    cout << "Going to sleep " << millis << " milliseconds." << endl;
    auto now = std::chrono::high_resolution_clock::now();
    SLEEP_MS(millis);
    auto then = chrono::high_resolution_clock::now();
    cout << "Milliseconds counted by std::chrono::high_resolution_clock: " << std::chrono::duration_cast<std::chrono::milliseconds>(then - now).count() << endl;
    return 0;
}
