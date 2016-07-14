#ifndef printArray_h
#define printArray_h

void printArray(int *A, int len);
void putsArray(int *A, int len);
void printArray(std::string A[], int len);
void putsArray(std::string A[], int len);

void printSubArray(int *A, int first, int last, bool indent = false);
void putsSubArray(int *A, int first, int last);
void putsArray(int A[], int len, const char *postLabel);
void putsArray(const char *preLabel, int A[], int len);
void putsArray(const char *preLabel, int A[], int len, const char *postLabel);

#endif // printArray_h