// ArrayAlgo.cpp
// Author: Sprax Lines
// If this looks Javanese, it's because I translated some of it from my 
// ArrayAlgo.java

#include "stdafx.h"
#include "ArrayAlgo.hpp"

#if 00
int ArrayAlgo::maxContiguousSum(int A[], unt len) 
{
    int maxSum = 0; 
    int maxNow = 0;
    for (unt j = 0; j < len; j++) {
        maxNow = max(0, maxNow+A[j]);
        maxSum = max(maxNow, maxSum);
    }
    return maxSum;
}

int ArrayAlgo::maxPositiveContiguousProduct(int A[], unt len) 
  {
    int maxPro = 0; 
    int maxNow = 1;
    for (unt j = 0; j < len; j++) {
      maxNow = max(1, maxNow * A[j]);
      maxPro = max(maxNow, maxPro);
    }
    return maxPro;
}



pair<int,int> ArrayAlgo::indicesContainsTwoEntriesThatSumToN(int arr[], size_t size, int sum) 
{
    hash_map<int, int> val2idx;
    for (size_t j = 1; j < size; j++) {
        int needed = sum - arr[j];
        hash_map<int,int>::const_iterator found = val2idx.find(needed);
        if (found != val2idx.end()) {
            return pair<int,int>(arr[j], j);
        }
        val2idx.insert(pair<int,int>(arr[j], j));
    }
    return pair<int,int>(-1,-1);
}

bool ArrayAlgo::arrayContainsTwoEntriesThatSumToN(int *pint, int len, int sum) 
{
    hash_set<int> seen;
    while ( --len >= 0 ) {
        if (seen.count(sum - *pint) > 0) {
            return true;
        }
        seen.insert(*pint++);
    }
    return false;
}

bool ArrayAlgo::test_arrayContainsTwoEntriesThatSumToN()
{
    int arr[] = { 1, 2, 3, 4, 5, 5, 7 };// N = 7
    bool does = arrayContainsTwoEntriesThatSumToN(arr, 7, 10);
    cout << "arrayContainsTwoEntriesThatSumToN got " << (does ? "true" : "false") << endl;
    return does;
}


#if 0
  
static int * maxMinContiguousProduct(int iA[], unt len) // TODO: seems to work -- test it more!
  {
    int maxProduct = 0, maxPositive = 0; 
    int minProduct = 0, minNegative = 0;
    int minTemp;
    for ( int ia : iA ) {
      if (ia > 0) {
        maxPositive = max(ia, ia * maxPositive);
        if (minNegative < 0)
          minNegative = ia * minNegative;
      } else if (ia < 0) {
        minTemp = min(ia, ia * maxPositive);
        if (maxPositive > 0)
          maxPositive = ia * minNegative;
        minNegative = minTemp;
      } else {                  // ia == 0
        maxPositive = 0;
        minNegative = 0;
        continue;
      }
      maxProduct  = max(maxPositive, maxProduct);
      minProduct  = min(minNegative, minProduct);
    }
    int ret[] = { maxProduct, minProduct };
    return ret;
  }  
  
  
  static double * maxAndMinContiguousProduct(double DD[]) // TODO: does not work?
  {
    double maxProduct  = 0.0, minProduct  = 0.0; 
    double maxPositive = 0.0, minNegative = 0.0;
    double maxTemp;
    for ( double dd : DD ) {
      if (dd > 0) {
        if (maxPositive > 0)
          maxPositive = max( 1, maxPositive * dd);
        minNegative = min(-1, minNegative * dd);
      } else if (dd < 0) {
        maxTemp     = max( 1, minNegative * dd);
        minNegative = min(-1, maxPositive * dd);
        maxPositive = maxTemp;
      } else { // dd == 0
        maxPositive =  0;
        minNegative =  0;
        continue;
      }
      maxProduct  = max(maxPositive, maxProduct);
      minProduct  = min(minNegative, minProduct);
    }
    double ret[] = { maxProduct, minProduct };
    return ret;
  }
  #endif


  
  //////////////////////////////////////// PAIRS //////////////////////////////////////////
  ArrayAlgo::IntPair ArrayAlgo::firstPairSumToN(int arr[], unt len, int sum)
  {
    IntPair pair(-1,-1);  // return values: invalid indices indicate failure
    std::map<int, int> val2idx;
    for (unt j = 0; j < len; j++) {
      // If there is a previously stored index i s.t. A[i] + A[j] == sum, return <i,j>
        if ( val2idx.find(sum - arr[j]) != val2idx.end() ) {
        pair.first = val2idx[sum - arr[j]];
        pair.second = j;
        break;
      }
      // Store only the first occurrence of each array value
      if ( val2idx.find(arr[j]) == val2idx.end() )
        val2idx.insert(std::map<int, int>::value_type(arr[j], j));
    }
    return pair;
  }
  
  int ArrayAlgo::test_pairs() 
  {
    //int iB[] = { 1, 2, 0, -2, 3, -4, 0 };
    int iB[] = { -1, 2, 1, 2, -3, 0, 4 };
    unt len = sizeof(iB)/sizeof(int);
    for (int sum = 0; sum < 10; sum += 3) {
      puts("\n");
      IntPair pair = firstPairSumToN(iB, len, sum);
      printArray(iB, len);
      printf(" => 1st pair to add up to %d: [%d, %d]\n", sum, pair.first, pair.second);
    }
    return 0;
  }  

 
 /////////////////////////////////// CONTIGUOUS SUBARRAYS ///////////////////////////////////////////
  /**
   * Naive algorithm: time = O(N^2), additional space = O(1)
   * @param arr Input array of pos and neg int's.
   * @param sum The number to which which the sub-array is to add up to. 
   * @return range = first and last index of the first sub-array adding
   * up to sum, where the first sub-array is the one that begins first,
   * not necessarily the one that ends first.  Thus in [2, 1, 0, -1, 2],
   * the first sub-array summing to 0 is [1, 0, -1] with first and last
   * indices <1,3>, and not [0] with indices <2, 2>.
   */
  int * ArrayAlgo::firstContiguousSumToN_naiveN2(const int arr[], unt len, int sum)
  {
    int range[] = {-1,-1};  // return values: invalid indices indicate failure (Java return idiom)
    for (unt j = 0; j < len; j++) {
      int partial = 0;
      for (unt k = j; k < len; k++) {
        partial += arr[k];
        if (partial == sum) {
          range[0] = j;
          range[1] = k;
          return range; // warning C4172: returning address of local variable or temporary (FIXME)
        }
      }
    }
    return range;       // warning C4172: returning address of local variable or temporary (FIXME)
  }

  int ArrayAlgo::test_contiguousSubArrays() 
  {
    //int iB[] = { 1, 2, 0, -2, 3, -4, 0 };
    int iB[] = { -1, 2, 1, 2, -3, 0, 4 };
    unt len  = sizeof(iB)/sizeof(int);
    for (int sum = 0; sum < 10; sum += 3) {
      puts("\n");
      int *pNaive  = firstContiguousSumToN_naiveN2(iB, len, sum);
      int naive[2] = { pNaive[0], pNaive[1] };

      printArray(iB, len);
      printf(" => 1st contiguous sum to %d: [%d, %d] (naive algo)", sum, naive[0], naive[1]);
      /*
      int range[] = firstContiguousSumToN(iB, sum);
      printArray(iB, len);
      " => 1st contiguous sum to " + sum + ": [" + range[0] + ", " + range[1] + "]");
      */
    }
    
////int stat = test_maxContiguousSubArray();

    return 0;
  }
  


  int ArrayAlgo::unit_test(int level) 
  {
    puts("ArrayAlgo.unit_test\n");   
    int stat = 0;
    
    stat += test_pairs();
    stat += test_contiguousSubArrays();    
    
    if (level > 1) {

      //stat += test_triplets();
      //stat += test_fullHouse();
      //stat += test_interleave();
      //stat += test_nonRandomShuffle();
      //stat += test_circleStart(5,1);
    }
    return stat; 
}
  
#if 0
  /**
   * 2-pass algorithm: time = O(N), additional space = O(N)
   * @param arr Input array of pos and neg int's.
   * @param sum The number to which which the subarray is to add up to. 
   * @return range = first and last index of the first sub-array adding
   * up to sum, where the first sub-array is the one that begins first,
   * not necessarily the one that ends first.  Thus in [2, 1, 0, -1, 2],
   * the first sub-array summing to 0 is [1, 0, -1] with first and last
   * indices <1,3>, and not [0] with indices <2, 2>.
   */
  static int *firstContiguousSumToN(final int arr[], int sum)
  {
    int range[] = {-1,-1};  // return values: invalid indices indicate failure
    Map<Integer, Integer> sum2idx = new HashMap<Integer, Integer>();
    sum2idx.put(0, -1);
    for (int partSum = 0, j = 0; j < arr.length; j++) {
      // compute the next partial sum up to and including A[j]
      partSum += arr[j];
      // If there is a previously stored index i s.t. i < j and pS[j] - pS[i] == sum, 
      // return <i+1,j>
      if ( sum2idx.containsKey(partSum - sum) ) {
        // The starting index of the sub-array is 1 after the end 
        // of the partial array whose partial sum we subtracted.
        range[0] = sum2idx.get(partSum - sum) + 1;
        range[1] = j;
        break;
      }
      // Store only the first occurrence of each partial sum
      if ( ! sum2idx.containsKey(partSum))
        sum2idx.put(partSum, j);    // j == index of last array entry in this partial sum
    }
    return range;
  }
  
  static int test_maxContiguousSubArray() {
    int iA[] = { 1, -2, 3, -5, 0, -4, 5, -4, 7, -9, 2, 1, 0 };
    int maxSum = maxContiguousSum(iA);
    S.putsArray(iA, " => max contiguous sum " + maxSum);
    int maxPro = maxPositiveContiguousProduct(iA);
    S.putsArray(iA, " => max positive contiguous entries product " + maxPro);
    int maxMin[] = maxMinContiguousProduct(iA);
    S.putsArray(iA, " => max & min contiguous product " + maxMin[0] + " and " + maxMin[1]);
    return 0;
  }
  


  ////////////////////////////////////// PERMUTATIONS & INDEX-MAPPINGS /////////////////////
  
  static void interleave3(int arr[])  // shuffle
  {
    int j, k, intemp, N = arr.length/3;
    int tmp[] = new int[arr.length];
    
    for (j = 0; j < 3; j++) {
      k = j*N;
      if (j < k) {
        intemp = arr[j];
        arr[j] = arr[k];
        arr[k] = intemp;
        tmp[j] = arr[k];
      }
    }
    arr = tmp;
  }
  
  static int test_interleave()
  {
    int N = 4;
    int arr[] = new int[3*N];
    for (int j = 0; j < N; j++) {
      arr[j      ] = 100 + j;
      arr[j + N  ] = 200 + j;
      arr[j + N*2] = 300 + j;
    }
    S.putsArray("Before:", arr);
    interleave3(arr);
    S.putsArray("After: ", arr);
    return 0;
  }
  
  /*
  Tags: Google » Algorithm(1st telephone interview)  » Application Developer
  Question #10103678 (Report Dup) | Edit
  Google Interview Question for Software Engineers about Arrays
  anonymous on August 04, 2011

  Given a sorted array, output all triplets <a,b,c> such that a-b = c. Expected time is O(n^2). 
  My approach using binary search took O(n^2 logn). 
  When you attempt an approach, test your code with this example 
  and list your outputs for verification. Thanks.
  -12, -7, -4, 0, 3, 5, 9, 10, 15, 16
  Tags: Google » Arrays  » Software Engineer
  Question #10094092 (Report Dup) | Edit
  Amazon Interview Question for Software Engineer / Developers about Algorithm
  bar raiser question on August 04, 2011  
  */
  /**
   * findDifferenceTripletsWithIndices
   * Given a const array of size N, output all triplets <a,b,c> such that a-b = c.
   * Expected time is O(N^2), additional space O(N).
   * 
   * Note: For this algorithm, a pre-sorted array with no duplicates is not 
   * expected to run significantly faster than a non-sorted one with duplicates.
   * 
   * If we know the input array is sorted, then all duplicates would be
   * contiguous, so we could just store their ranges, 
   * replaceing: Map<Integer, ArrayList<Integer>> val2idxList
   * with:       Map<Integer, Integer[]>> val2idxRange 
   * (and even replace the Integer[] with Pair<Integer> if you have it).
   * But what that saves is not so significant.
   * 
   * If we know there are no duplicate values in the input array, we can 
   * replace: Map<Integer, ArrayList<Integer>> val2idxList
   * with:    Map<Integer, Integer>            val2idx
   * That's better, but of course it's still O(N).
   * 
   * If the additional space is limited to O(1), we could use binary search 
   * to look up the diff's in place, but then the expected time is O(N^2 log N).  
   * Storing anything to make the lookup faster will be at least O(N) space 
   * (or O(N^2) if you store differences), so don't do it.  
   * The hashmap solution is good enough.
   * 
   */
  static void findDifferenceTripletsWithIndices(final int arr[])
  {
    // 1st pass: Map each array value to a list of indices for that value 
    Map<Integer, ArrayList<Integer>> val2idxList = new HashMap<Integer, ArrayList<Integer>>();
    for (int j = 0; j < arr.length; j++) {
      if ( ! val2idxList.containsKey(arr[j])) {
        val2idxList.put(arr[j], new ArrayList<Integer>());
      }
      val2idxList.get(arr[j]).add(j);
    }    
    // 2nd pass: for each difference (with indices j & k), 
    // lookup set of indices {m} for array value == difference 
    // and print the triples <j, k, {m}>
    for (int q = 0, j = 0; j < arr.length; j++) {
      for (int k = 0; k < arr.length; k++) {
        int dif = arr[j] - arr[k];
        if (val2idxList.containsKey(dif)) {
          for ( int idx : val2idxList.get(dif)) {
            System.out.format("Index triplet %2d <%d, %d, %d> gives % 3d - % 3d = % 3d\n", ++q, j, k, idx, arr[j], arr[k], dif);
          }
        }
      }
    }
  }
  
  /**
   * Given a const array of size N, output all triplets <a,b,c> 
   * such that a - b = c and a != b and a != c and b != c.
   * Expected time is O(N^2), additional space O(N).
   * @param arr
   */
  static void findDistinctDifferenceTriplets(int A[])
  {
    Map<Integer, ArrayList<Integer>> val2idxList = new HashMap<Integer, ArrayList<Integer>>();
    for (int j = 0; j < A.length; j++) {
      if (A[j] == 0)
        continue;                             // Because a - b == 0 implies a == b
      if ( ! val2idxList.containsKey(A[j]))
        val2idxList.put(A[j], new ArrayList<Integer>());
      val2idxList.get(A[j]).add(j);
    }
    for (int q = 0, j = 0; j < A.length; j++) {
      // Note: (A[j] == 0) is OK, because if a == 0, then a - b == c implies -b == c, not that b == c (and we know c != -c)
      for (int k = 0; k < A.length; k++) {  // Can't just use k > j, because subtraction is non-commutative.
        if (k == j || A[k] == 0)
          continue;                           // Because if b == 0, then a - b == c implies a == c
        int dif = A[j] - A[k];
        if (val2idxList.containsKey(dif)) {
          for ( int idx : val2idxList.get(dif)) {
            if (A[j] != dif && A[k] != dif) {
              System.out.format("Index triplet %2d <%d, %d, %d> gives % 3d - % 3d = % 3d\n", ++q, j, k, idx, A[j], A[k], dif);
            }
          }
        }
      }
    }
  }
  
  static int test_findDifferenceTripletsIndices() 
  {
    int A[] = { -12, -7, -4, 0, 3, 5, 9, 10, 15, 16 };
    S.putsArray("findDistinctDifferenceTriplets", A, "");
    findDifferenceTripletsWithIndices(A);
    S.putsArray("findDistinctDifferenceTriplets", A, "");
    findDistinctDifferenceTriplets(A);

    int B[] = { -12, -7, -4, 0, 4, 5, 5, 9, 10, 15, 16 };
    S.putsArray("findDistinctDifferenceTriplets", B, "");
    findDistinctDifferenceTriplets(B);    
    return 0;
  }
  
  static int test_triplets() 
  {
    return test_findDifferenceTripletsIndices();
  }    
  
  

  /** 
   * isFullHousePSX uses XOR, SUM, and PRODUCT plus some arithmetic 
   * to determine if the input is 3 of one int and 2 of another (like
   * a full house).  
   * Pros: One pass, minimal extra storage.
   * Cons: Fails to distinguish 5 of a kind from 3 and 2 of a kind.
   *       Can give false positive if one of the inputs is 0 (because then 
   *       product==0, so if x or y == 0, x*x*x*y*y is also 0).
   *      It seems like only 3 equations are being used to determine 5 quantities.
   *      
   * Counter examples: [2,2,3,3,6] [3,3,10,10,15], etc.
   * Your methods also fails if 0 is allowed, as in [0,1,1,1,1]. 
   * Your method does not distinguish between 5 of a kind and a 
   * full house, though to be fair, neither did the question. 
   * It's probably best to have asked the interviewer. To minimize complexity 
   * (not maximize cleverness), you might as well just count how many distinct 
   * values there are, as well as how many of each value. You can return false 
   * in less than one complete pass, depending on the input.     
   *      
   * @return true if intput is 2 of one int and 3 of another.
   */
  static boolean isFullHousePSX(int n1, int n2, int n3, int n4, int n5, boolean bPrint)
  {
    int Z[] = {n1, n2, n3, n4, n5};
    int x = 0, y;
    int sum = 0, product = 1;
    
    for (int i=0; i<5; i++){
      x   ^= Z[i];
      sum += Z[i];
      product *= Z[i];
    }
    y = (sum - 3*x)/2;
    
    boolean bFullHouse = ( x*x*x * y*y == product );
    if (bPrint)
      S.putsArray(Z, " ==> fullHouse = " + bFullHouse + " with x = " + x + " and y = " + y);  
    return bFullHouse;
  }

  /**
   * isFullHouseMap uses a map to count the occurrances each value in the input.
   * The input is deemed a "full house" if the map ends up with exactly 2 keys
   * and the values 2 and 3.
   * Pros: distinguishes full house from five of a kind.
   * Cons: Overhead of managing a small map.  But if the app needs to check
   * many inputs, of course the map can be made persistent and just cleared
   * before each usage.  (Not static, but a different map owned by each instance.)
   */
  static boolean isFullHouseMap(int n1, int n2, int n3, int n4, int n5)
  {
    int Z[] = {n1, n2, n3, n4, n5};
    Map<Integer, Integer> counts = new HashMap<Integer, Integer>();
    for (int z : Z) {
      if ( ! counts.containsKey(z))
        counts.put(z, 1);
      else {
        int count = 1 + counts.get(z);
        counts.put(z, count );
      }
    }
    if (counts.keySet().size() == 2 && counts.containsValue(2) && counts.containsValue(3))
      return true;
    return false;
  }
  
  /**
   * isFullHouseMap uses a map to count the occurrances each value in the input.
   * The input is deemed a "full house" if the map ends up with exactly 2 keys
   * and the values 2 and 3.
   * Pros: distinguishes full house from five of a kind.
   * Cons: Overhead of managing a small map.  But if the app needs to check
   * many inputs, of course the map can be made persistent and just cleared
   * before each usage.  (Not static, but a different map owned by each instance.)
   */
  static boolean isFullHouseCount(int n1, int n2, int n3, int n4, int n5)
  {
    int Z[] = {n1, n2, n3, n4, n5};
    int countFirst = 1, countOther = 0, valueOther = 0;
    for (int j = 1; j < 5; j++) {
      if (Z[j] == Z[0]) {
        ++countFirst;
      } else {
        ++countOther;
        if (countOther == 1)
          valueOther = Z[j];
        else if (valueOther != Z[j]) {
          return false;               // Found a 3rd value
        }
      }
    }
    // Still here?  Then there are at most two values.
    // If we letting 5-of-a-kind count as a fullhouse, then also check for countFirst == 5 
    if ((countFirst == 2 && countOther == 3) || (countFirst == 3 && countOther == 2))
      return true;
    return false;
  }
  
  
  
  /**
   * @return
   */
  static int test_fullHouse()
  {
    isFullHousePSX( 3,2,3,2, 3, true);
    isFullHousePSX(-3,2,3,2,-3, true);
    isFullHousePSX(-3,2,-3,2,-3, true);
    isFullHousePSX( 5, 5, 5, 5, 5, true);
    isFullHousePSX( 2,2,2, 4,4, true);
    isFullHousePSX( 4, 4, 4, 4, 4, true);
    isFullHousePSX( 2, 2, 3, 3, 6, true);
    
    int max = 24;
    for(int j = 0; ++j < max; ) {
      for(int k = 0; ++k < max; ) {
        for(int l = 0; ++l < max; ) {
          for(int m = 0; ++m < max; ) {
            for(int n = 0; ++n < max; ) {
              boolean bMap = isFullHouseMap(  j,k,l,m,n);
              boolean bCnt = isFullHouseCount(j,k,l,m,n);
              boolean bPsx = isFullHousePSX(  j,k,l,m,n, false);
              if (bMap != bCnt || bMap != bPsx) {
                System.out.format("Failure at %2d %2d %2d %2d %2d\n", j,k,l,m,n);
                bMap = isFullHouseMap(  j,k,l,m,n);
                bCnt = isFullHouseCount(j,k,l,m,n);
                bPsx = isFullHousePSX(  j,k,l,m,n, true);
              }
            }
          }
        }
      }
    }
    return 0;
  }
  
  
  

  /** TODO
  Amazon Interview Question for Software Engineer / Developers about Coding
  aniketnit on October 24, 2011

  Suppose there is a circle. You have five points on that circle. Each
  point corresponds to a petrol pump. You are given two sets of data.

  1. The amount of petrol that petrol pump will give.
  2. Distance from that petrol pump tp the next petrol pump.

  (Assume for 1 lit Petrol the truck will go 1 km)

  Now calculate the first point from where a truck will be able to
  complete the circle.
  (The truck will stop at each petrol pump and it has infinite
  capacity).
  Give o(n) solution. You may use o(n) extra space.
  
  * IDEA: for each point on the circle, calculate how mucy excess gas
  * you'd have if you had started at this point and made it all the
  * way arond and back to it, stopping at each of the other points.
  * If the running excess at point j is < 0, set first = j+1, and if
  * first > len, that means it cannot be done.  
  *
  */

  static int circleStart(const double gas[], final double dst[], double mpg)
  {
    int length  = gas.length;
    if (length != dst.length) {
      System.out.format("cirlceStart: different array lenghts %d and %d\n"
          , gas.length, dst.length);
      return -2;
    }
    double excessGasHere, excessGasTemp = 0.0, excessGasTotal = 0.0;
    int start = 0;
    // 1st pass: get local and total excesses of gas compared to miles
    for (int j = 0; j < length; j++) {
      excessGasHere   = gas[j] - dst[j]/mpg;
      excessGasTemp  += excessGasHere;
      excessGasTotal += excessGasHere;
      if (excessGasTemp < 0.0) {  // If, having started at start, we would not
        excessGasTemp = 0.0;      // get enough gas here to make it to the next
        start = j + 1;            // station, then try tne next possible starting 
      }                           // point, and reset the running excess to zero.
    } 
    // For any station k s.t. old start < k < new start, we'd also run out
    // of gas before arriving at station j+1, because the excess from old 
    // start to k had to be >= 0, so starting at k instead of old start 
    // could only result in less excess at j, that is, not enough gas to
    // get to station j + 1.
    if (excessGasTotal < 0.0) { // Less than enough total gas for total idstance?
      return -1;                // Then no starting point will work.
    }                     
    // Still here?  We know there is enough gas, so there is 
    // definitely a solution, and start has now been set to the first
    // such solution.  The proof is an easy argument by contradiction.
    return start;
  }    
    
  static int test_circleStart(int numPoints, int numTrials)
  {
    double gas[] = new double[numPoints], gasTot = 0;;
    double dst[] = new double[numPoints], dstTot = 0;
    for (int j = 0; j < numPoints; j++) {
      gas[j] =      1.0 + Math.cos(j);
      dst[j] = 1.0;
      gasTot += gas[j];
      dstTot += dst[j];
    }
    gas[3] += gas[0];
    gas[2] -= gas[0];
    if (dstTot > gasTot) {
      double dif = dstTot - gasTot + 0.01;
      gas[numPoints-1] += dif;
      gasTot += dif;
    }
    double mpg = 1.0;
    int start = circleStart(gas, dst, mpg);
    S.putsArray("circleStart gas:", gas, " ("+gasTot+")");
    S.putsArray("       distance:", dst, " ("+dstTot+") got start = " + start);
    
    double G[] = {  1,  2,  3,  4,  5 };
    double D[] = {  4,  1,  2,  3,  4 };
    double F[] = { -3, -2, -1,  0,  1 };      // excess starting at index 0
    double H[] = {      1,  2,  3,  4,  1 };  //        starting at index 1
    start = circleStart(G, D, mpg);
    S.putsArray("circleStart gas:", G, " ("+gasTot+")");
    S.putsArray("       distance:", D, " ("+dstTot+") got start = " + start);
    return min(0, start);
  }
  

  
  /**
   * Question :
   * Suppose we have an array like
   * 1,2,3,4,5,a,b,c,d,e where we have always even number of elements.First half of the elements
   * are integers and second half are alphabets we have to change it to like
   * 1,a,2,b,3,c,4,d,5,e in place i.e no use of any extra space, variables are allowed ..
   *
   */
  
  /** in-shuffle solution:
 
  Step 1) Find an m such that 2m+1 = 3**k <= 2n < 3**(k+1)
  Step 2) Right cyclic shift A[m+1 ... n+m] by m.
  Step 3) Foreach i = 0 to k - 1, considering only the Array[1...2m] start at 3i and 'follow the cycle'.  
  Step 4) Recurse  on A[2m+1...2n] 
 */
  
  
  static void inShuffleArray(char[] A)
  {
    int len = A.length;
    if (len % 2 == 1 || len < 10) {
      S.puts("inShuffArray: A.length must be even and >= 10");
      return;
    }
    
    int m = 0;
    for (int k = 2, pow3_k = 9, pow3_kp1 = 27; k < 8; k++) { 
      if (pow3_k <= len && len < pow3_kp1) {
        m = (pow3_k - 1) / 2;
        break; 
      }
    } // TODO: what if 3**8 isn't big enough?
    
    
  }
  
  
  static void mixArray(char[] A, int start, int end){
    assert((end - start + 1) % 2 == 0);
    if (start +1 >= end)
      return;
    
    // swap the every element after start with (end - start)/2 th item after it
    // 1 2 3 4 5 a b c d e  becomes 1 a b c d 2 3 4 5 e
    int distance = (end - start) / 2;
    for(int i = start + 1; i + distance < end; i++) {
      intArraySwap(A, i, i + distance); 
    }
    
    // swap element back to origin place except head and tail
    // 1 a b c d 2 3 4 5 e becomes 1 a 2 3 4 b c d 5 e
    distance = (end - start - 2) / 2;
    for(int i = start + 2; i + distance < end - 1; i++) {
      intArraySwap(A, i, i + distance);
    }
    
    // recursive call to mix the rest
    mixArray(A, start + 2, end - 2);
  }
  
  static void intArraySwap(char[] A, int i, int j) {
    char temp = A[i];
    A[i] = A[j];
    A[j] = temp;
    return;
  }
  
  static void test_mixArrayShuffle(char A[])
  {
    S.putsArray("mixArray(", A, ") ==>");
    mixArray(A, 0, A.length -1);
    S.putsArray("         ", A);
  }
  
  static int test_nonRandomShuffle()
  {
    char[] A = "123456789abcdefghi".toCharArray();
    test_mixArrayShuffle(A);
    int len = 21;

    char[] B = new char[len*2];
//    // pre-shuffled
//    for (int j = len*2; --j >= 0; ) {
//      B[j--] = (char)(j + 'a');
//      B[j]   = (char)(j + 'A');
//    }
    
    // un-shuffled
    for (int j = 0; j < len; j++)
      B[j] = (char)(j + 'A');
    for (int j = 0; j < len; j++)
      B[j+len] = (char)(j + 'a');
    test_mixArrayShuffle(B);
    
    inShuffleArray(B);
    return 0;
  }
#endif
  
  


/*
// It's also possible the number of removals may exceed the maximum
// expression count allowed by oracle.
// The number of property values governs the number of expressions emitted.
while (endNdx < propArray.length){
  // the deletion is done from startNdx up to but not including endNdx
  endNdx = (endNdx+mMaximumExpressions < propArray.length)
    ? (endNdx+mMaximumExpressions) : propArray.length;
  result += deleteRange(pItemId, pProperty, propArray, startNdx, endNdx, pConnection);
  startNdx = endNdx;
}
*/


    /*
   * 
you have an array of integers, find the longest
subarray which consists of numbers that can be arranged in a sequence, e.g.:

a = {4,5,1,5,7,4,3,6,3,1,9}
max subarray = {5,7,4,3,6}
10
Country: -
Tags: Microsoft » Algorithm
Question #11256218 (Report Dup) | Edit | History


0
of 0 vote
sumit.gupta23121988 on October 19, 2011 |Edit | Edit

can u give the solution of this
Reply to Comment
0
of 0 vote
Aditya H on October 19, 2011 |Edit | Edit

What do you mean by numbers that can be arranged in a sequence, all numbers can be arranged in a sequence
Anonymous on October 19, 2011 |Edit | Edit

consecutive elements sequence... this is a google interview question... this was discussed earlier.
Anonymous on October 19, 2011 |Edit | Edit

can you please provide discussion link?
Anonymous on October 19, 2011 |Edit | Edit

id=9783960
Anonymous on October 20, 2011 |Edit | Edit

I think the Google interview asked for a subset of the array, this question asks for a subarray. The obvious solution is checking all possible subarrays, but there must be a better solution...
asm on October 20, 2011 |Edit | Edit

'id=9783960' it's a different problem.
Here it is asked to find a SUBARRAY (contiguous part) that can be transformed to a sequence of consecutive integers, i.e.:

a = {4,5,1,5,7,4,3,6,3,1,9}
the subarray is: {5,7,4,3,6} because these numbers can be arranged as: {3,4,5,6,7}
Anonymous on October 20, 2011 |Edit | Edit

ok, here is an algorithm I had in mind:

The idea is if the numbers of a subarray can be arranged in consecutive way that their sum can be evaluated as follows:
max*(max+1)/2 - min*(min-1)/2
where 'max' and 'min' are the maximal and minimal numbers in a subarray.

So the algorithm is go through the array updating 'max' and 'min', as well as the current sum.
In each step we check if computed sum equals to the value returned by the formula above.
If so, we have found a subarray with the given properties.
We keep the longest one seen so far.

Since we have to consider all possible array suffixes, the complexity is O(n^2)

   */
  
  /**
 * Sprax sez.........................................................
 * First non-repeading char in string:
 * 1) O(N) time, O(1-bit per alphabet char) space.
 * 2) Now how about O(1-bit per string char) space?
   Answer to 1) 2-passes using 2 half-alphabet bitmasks, one for 1st occurrence, 2nd for repeat.
   Each pass gives a min index.  Take the smaller of these two.
   Answer to 2) ??
 * 
 * ==================================================================
 */

/*
2
Country: India
Tags: Amazon » Coding  » Software Engineer / Developer
Question #11316872 (Report Dup) | Edit
Amazon Interview Question for Software Engineer / Developers about Algorithm
subharaj.manna on October 24, 2011

Given a matrix you have to find the shortest path from one point to another within the matrix. The cost of path is all the matrix entries on the way. You can move in any direction (up, down, left, right, diagonally)

e.g.

5 9 10 1
3 7 4 4
8 2 1 9

So shortest path from (0,0) to (2,2) is (0,0)--(1,1)---(2,2). Path cost - 5+7+1 = 13 (wrong)


Write a function that returns true if it not returned n trues in last k seconds in O(1) time.
Question #11323723 (Report Dup) | Edit
Interview Question
ivishnuvardhan on October 24, 2011

Design maze data structure and find path between 2 cells with modularity and extensibility.

[Full Interview Report]
Country: -
Tags:
Question #11316750 (Report Dup) | Edit
Interview Question
ivishnuvardhan on October 24, 2011

A white cell has been given. Every second, the white cell gets divided into 9 parts in which middle one is black and all other remains as white. Black cell (if any) also gets divided into 9 cells which are all black. Given time passed, x(horizontal) and y(vertical) indices of the cell, find the color of the cell. Ex: timepaased as 1 and x,y(index starts from 0) as 2,2, color is white. For timepassed as 2, x,y as 4,1, color is black.

[Full Interview Report]
Country: -
Tags:
Question #11322737 (Report Dup) | Edit
Interview Question about Algorithm
arindam.mitra2 on October 23, 2011

You are given a tree (an undirected acyclic connected graph) with N nodes, and edges numbered 1, 2,
3...N-1. Each edge has an integer value assigned to it, representing its length.
We will ask you to perfrom some instructions of the following form:
DIST a b : ask for the distance between node a and node b
or
KTH a b k : ask for the k-th node on the path from node a to node b.
1
Country: -
 */
/** TODO
 * 

You have given an array give a function which returns a boolean 
telling whether the numbers in the array are consecutive or not.

Say given array is A[1...n].

Find max, find min. Check if max - min = n-1.

If max - min = n-1, then we can now have a boolean array of size n, where we check off each number in the interval [min, max].
anshulzunke on September 16, 2011 |Edit | Edit

I had given the same answer and interviewer replied "Are you sure this gonna work for all cases?"
Anonymous on September 17, 2011 |Edit | Edit




I gave 2 solutions

1. sort the array
2. use another array of booleans and then check uniqueness of each number but space complexity O(N)
3. i thought of it but could nt complete. Using the arithemetic progression formulas
Anonymous on September 15, 2011 |Edit | Edit

So you are looking for an O(1) space solution?
anshulzunke on September 16, 2011 |Edit | Edit



Multiply and Addition approach will not work anyway.
Reply to Comment
0
of 0 vote
anonymous on September 16, 2011 |Edit | Edit

find the min element and sum of all the elements in the array. Then use AM formula where a=min element and n=arraysize and d=1.
anshulzunke on September 16, 2011 |Edit | Edit

Are you sure this will work??
anonymous on September 16, 2011 |Edit | Edit

i hope so, can you provide any case where it fails?
anshulzunke on September 16, 2011 |Edit | Edit

1 2 3 4 5
1 3 3 3 5
Ady on September 16, 2011 |Edit | Edit

To put it in a better way:

1) Find the min element and sum of all the elements in the array.
2) Then use Arithmetic series formula where a=min element and n=arraysize and d=1.

Formula: Sn= n/2[2a+(n-1)d]

3) Compare the sum(from step 1) with the above result. If its the same then the numbers are consecutive.

I hope this is right.
anshulzunke on September 16, 2011 |Edit | Edit

It wont work.
I had given u an example. Both the series have same Sum and Min Value but 1 is consecutive and other is not
Ady on September 16, 2011 |Edit | Edit

Yes. You are right.

Then how about adding Step 4 where we check for uniqueness of elements in the array - O(n)
anshulzunke on September 16, 2011 |Edit | Edit

BTW i had given the exact same answer and both Interviewer and i knew it wasnt the right answer. Infact i had gone a step further by finding the Max and compared it with the max coming in the series

Max = min + (n - 1)* 1

The approach is surely right but i dont think answer will be correct
Reply to Comment
0
of 0 vote
anshulzunke on September 16, 2011 |Edit | Edit

Uniqueness is the key here. If you get a solution for checking uniqueness of N elements between max and min then u have done ur job.

For the uniqueness only i had suggested having another array but again space complexity would be O(N)
lol on September 18, 2011 |Edit | Edit

True. But NO solution exists which is O(n) time and O(1) space complexity to test uniqueness. Look up in Wikipedia.
lol on September 18, 2011 |Edit | Edit

In general, uniquess test takes theta(nlogn). For integer data type, O(n) solution is available.

Read this: en.wikipedia.org/wiki/Element_distinctness_problem
O(n) sol: stackoverflow.com/questions/177118/algorithm-to-determine-if-array-contains-n-nm
 */


/**
 * TODO
 * 

You have two hashmaps HM1 and HM2 where key = Id(long) value = timestamp. You need to give a program to return a list of Ids combined from both the hashmaps such that they are sorted as per their timestamps

[Full Interview Report]
Country: India
Interview Type: Phone Interview
Tags: Tribal Fusion » Java  » Software Engineer
Question #10650801 (Report Dup) | Edit
Tribal Fusion Interview Question for Software Engineers about Java
anshulzunke on September 17, 2011

How will you make your own Hashmap class?
1
[Full Interview Report]
Country: India
Interview Type: Phone Interview
Tags: Tribal Fusion » Java  » Software Engineer
Question #10650800 (Report Dup) | Edit
Microsoft Interview Question for Software Engineer / Developers about Algorithm C C++ Coding, Data Structures, Algorithms
anmolkapoormail on September 17, 2011

Given an array arr[] of n integers, construct a Product Array prod[] (of same size) such that prod[i] is equal to the product of all the elements of arr[] except arr[i]. Solve it without division operator and in O(n).
13

 * @author slines
 *
 */
/*
 * 
 * TOODO
 * 

How do you go about finding the top 10 google searches at the end of a day? In case you are storing the information about them, how and where would you store it? (like disks etc)
5
[Full Interview Report]


This was a question asked in an onsite interview with amazon. The interviewer had to be the bar raiser because every other question was pretty easy.

given a number, come up with all of the possible ways to insert '+' and '-' in that number. for example given 123, possible answer would be

1+23
1+2+3
1-23
1-2+3
1-2-3
1+2-3
12+3
12-3

20
Tags: Amazon » Algorithm  » Software Engineer / Developer
Question #10092001 (Report Dup) | Edit
Chronus Interview Question for Software Engineer / Developers about Arrays
keerthy on August 03, 2011

Given an array containing elements 1..N but containing repeated elements.we have to find the missing element..No extra spaces allowed and time complexity should be O(n).
9
[Full Interview Report]
Tags: Chronus » Arrays  » Software Engineer / Developer
Question #10102784 (Report Dup) | Edit
Chronus Interview Question for Software Engineer / Developers about Trees and Graphs
keerthy on August 03, 2011

Given a binary tree we have to update another pointer sibling in the node such that every node sibling is the left node of current node.If there is no left node then its sibling should point to the right most node at that level...!!
4
[Full Interview Report]


Given an array of elements and an integer x, rearrange the array such that all the elements greater than or equal to x should be in descending order and all the elements less than x should be in ascending order in most efficient manner.
6
[Full Interview Report]
Country: India
Interview Type: In-Person
Tags: Ittiam Systems » Algorithm  » Development Engineer
Question #10537074 (Report Dup) | Edit | History


0
of 0 vote
ACEpratap on September 11, 2011 |Edit | Edit

1:Mark the element 'x' as key element and take it to one of the two corners of an array.
2:Arrange elements in such fashion that elements to its left are smaller than the key element and right are greater or vice-versa (requires atmost n comparisons).
3:Have 2 BST 1 for elements smaller than key element and other for elements greator than or equal to key element.
4:Have inorder traversal for ascending order.
5:And put elements into stack first by having inorder traversal and then pop out for descending order.
Reply to Comment
0
of 0 vote
amar on September 11, 2011 |Edit | Edit

Sort the array using quick sort in descending order
Find the index of key element in the array
Reverse the array from key elements' index + 1 element to the last element to get the ascending order
Anonymous on September 12, 2011 |Edit | Edit

The second step has to take possible multi appearance of the key into account.
Reply to Comment
0
of 0 vote
Neeraj on September 12, 2011 |Edit | Edit

O(NlogN) algo.

-> Write your partition function with pivot as x( x may not be present in the array)

-> Partition function returns index k such that all elements in [0,k] are less than 'x' and all elements in range [k+1, N-1] are greater than 'x' .

->Now sort [0,k] range in ascending and [k+1, N-1] in descending order using Quick Sort or some other O(NlogN) algo.
Reply to Comment
0
of 0 vote
Anonymous on September 15, 2011 |Edit | Edit

Question doesn't asks you to have array as "all less elements followed by greater elements". It may be mix as well but order for less and greater elements has to be maintained among themselves.
Reply to Comment
0
of 0 vote
Anonymous on September 21, 2011 |Edit | Edit

Quick sort and then take i+1 to n elements and reverse them. Here i is the index for the given element.
 */

#endif // 00