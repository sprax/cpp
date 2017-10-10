// rank.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <chrono>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#define SLEEP_MS(A) Sleep(A)
#else
#define SLEEP_MS(A) usleep(A * 1000)
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

