#ifndef maxSubArrays_h
#define maxSubArrays_h

int max_subarray(int A[], int countA);
int Kadane(int arr[], int arrLen, int* pFirst, int* pLast);

int test_Kadane(void);  
int test_circleStart(int numPoints, int numTrials);

#endif // maxSubArrays_h