// spraxUtil.hpp : common functions
// Sprax Lines,  July 2010

#ifndef arrayUtil_hpp
#define arrayUtil_hpp

template <typename T> 
void swapArray(T A[], int j, int k)
{
    T temp = A[j];
    A[j]   = A[k];
    A[k]   = temp;
    return;
}

template <typename T>
bool getMinAndMax(const T A[], size_t len, T& minVal, T& maxVal)
{
    if (A == NULL || len < 1)
        return false;

    minVal = A[0];
    maxVal = minVal;
    if (len < 2)
        return true;
    
    for (size_t j = 1; j < len; j++) {
        if (minVal > A[j])
            minVal = A[j];
        else if (maxVal < A[j])
            maxVal = A[j];
    }
    return true;
}

template <typename T> 
void randomShuffle(T array[], int len, unsigned int seed = 0)
{
    if (seed == 0)
        seed = rand();
    srand(seed);
    for (int k, n = len; n > 1; ) { // n = number of items left to shuffle.
        k = rand() % n;             // 0 <= k < n.
        n--;                        // n is now the last pertinent index;
        T   temp = array[n];        // swap array[n] with array[k] (does nothing if k == n).
        array[n] = array[k];
        array[k] = temp;
    }
}

#endif // arrayUtil_hpp
