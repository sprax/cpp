// FileReader.cpp 
//
#include "stdafx.h"
#include <iostream>
using namespace std;

static const int SIZE_4K = 4096;

/** Given an open FILE pointer, read up to 4K bytes */
static int read4K(FILE *file, char *buf)
{
    return fread(buf, sizeof(char), SIZE_4K, file);
}

class FileReader
{
private:
    FILE *mFile;

public:
    FileReader(FILE *file) 
    {
        if (file == nullptr)
            throw "null file ptr";

        mFile = file; 
    }

    static void copyChars(char *dst, char *src, int len) {
        for (int j = 0; j < len; j++)
            *dst++ = *src++;
    }

    /** Read up to count chars into buf using read4K;
     *  return the number of chara actually read.
     */
    int readFile(int count, char *buf)
    {
        if (count < 1 || buf == nullptr)
            return 0;

        char tempBuf[SIZE_4K];
        int totalCharsCopied = 0;
        for (;;) {
            int numRead = read4K(mFile, tempBuf);

            // If we're at the end of the file, return the number already read.
            if (numRead == 0)
                return totalCharsCopied;

            // If the number of chars we just read + the number of chars we 
            // already read is >= count, finish filling buf with count chars
            // and return.
            if (numRead + totalCharsCopied >= count) {
                copyChars(&buf[totalCharsCopied], tempBuf, count - totalCharsCopied);
                return count;
            }

            // Otherwise, copy all the newly read chars into the buf
            // and keep going.
            copyChars(&buf[totalCharsCopied], tempBuf, numRead);
            totalCharsCopied += numRead;
        }
    }

    static int unit_test(const char *fileSpec)
    {
        cout << "BEGIN FileReader::unit_test(" << fileSpec << ")" << endl;
        FILE *file = nullptr;
        int status = 0;
        try {
            FILE *file = nullptr;
            fopen_s(&file, fileSpec, "r");
            FileReader reader(file);
            const int  SIZE_5K = 1024*5;
            char buf5k[SIZE_5K];

            int counts[] = { -1, 0, 1, SIZE_4K, SIZE_4K+1, SIZE_5K-1, SIZE_5K };
            int numCounts = sizeof(counts)/sizeof(int);
            for ( int j = 0; j < numCounts; j++) {
                int count = counts[j];
                int read  = reader.readFile(count, buf5k);
                if (count >= 0 && count != read) {
                    cout << "FileReader::readFile failed on count: " << count << endl;
                    status -= 1;
                }
            }
            fseek(file, 0, 0);
            reader.readFile(SIZE_5K, buf5k);
            char tmp5k[SIZE_5K];
            fseek(file, 0, 0);
            fread(tmp5k, sizeof(char), SIZE_5K, file);
            int cmp = strncmp(buf5k, tmp5k, SIZE_5K);
            if (cmp != 0) {
                cout << "FileReader::readFile fails on comparison with fread: " << cmp << endl;
                status -= 10;
            }

            // compare 

            fclose(file);
        } catch (...) {
            cout << "I/O error on file: " << fileSpec << endl;
            status = -100;
        }
        cout << "END   FileReader::unit_test" << endl;
        return status;
    }
};

int test_FileReader()
{
    return FileReader::unit_test("../../text/mobydick.txt");
}