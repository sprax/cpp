
#include "stdafx.h"
#include "printArray.h"

void printArray(int *A, int len) {
    for (int j = 0; j < len; j++)
        printf(" % 2d", A[j]);
}

void printArray(std::string A[], int len) {
    for (int j = 0; j < len; j++)
        printf(" % s", A[j].c_str());
}

void putsArray(int *A, int len)
{
    printArray(A, len);
    puts("\n");
}


void putsArray(std::string A[], int len)
{
    printArray(A, len);
    puts("\n");
}

void printSubArray(int *A, int first, int last, bool indent)
{
    if (indent) {
        for (int j = 0; j < first; j++)
            printf("   ");
    }
    for (int j = first; j < last; j++)
        printf(" % 2d", A[j]);
}  

void putsSubArray(int *A, int first, int last) 
{
    printSubArray(A, first, last);
    puts("\n");
}

void putsArray(int A[], int len, const char *postLabel) 
{
    printArray(A, len);
    puts(postLabel);
}

void putsArray(const char *preLabel, int A[], int len) 
{
    printf(preLabel);
    putsArray(A, len);
}

void putsArray(const char *preLabel, int A[], int len, const char *postLabel) 
{
    printf(preLabel);
    putsArray(A, len, postLabel);
}
