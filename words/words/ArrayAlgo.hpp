

#include <unordered_map>
#include <unordered_set>

using namespace std;

typedef unsigned int unt;

class ArrayAlgo
{
public:
    struct IntPair {
        int first;
        int second;
        IntPair(int f, int s) : first(f), second(s) {}
    };

    inline static int max(int a, int b) {
        return a > b ? a : b;
    }
    inline static int min(int a, int b) {
        return a < b ? a : b;
    }

    int maxContiguousSum(int A[], unt len);
    int maxPositiveContiguousProduct(int A[], unt len);
    bool test_arrayContainsTwoEntriesThatSumToN();
    static IntPair firstPairSumToN(int arr[], unt len, int sum);

    
    /////////////////////////////////// CONTIGUOUS SUBARRAYS ///////////////////////////////////////////


	static int firstContiguousSumToN_naiveN2(const int arr[], unt len, int sum, int &rFirst, int &rLast);
      
    pair<int,int> indicesContainsTwoEntriesThatSumToN(int arr[], size_t size, int sum);
    bool arrayContainsTwoEntriesThatSumToN(int *pint, int len, int sum);


        //////////////////////////////////////// PAIRS //////////////////////////////////////////


    static int test_pairs();
    static int test_contiguousSubArrays();
    static int unit_test(int level);

    static void maxMinContiguousProduct(int iA[], unt len, int& maxProduct, int& minProduct) // TODO: seems to work -- test it more!
    {
		maxProduct = 0;
		minProduct = 0;
		int	maxPositive = 0, minNegative = 0;
        int minTemp;
		for (unt j = 0; j < len; j++) 
		{
			int ia = iA[j];
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
    }  


#if 0
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

    static int test_contiguousSubArrays() 
    {
        //int iB[] = { 1, 2, 0, -2, 3, -4, 0 };
        int iB[] = { -1, 2, 1, 2, -3, 0, 4 };
        for (int sum = 0; sum < 10; sum += 3) {
            puts("\n");
            int naive[] = firstContiguousSumToN_naiveN2(iB, sum);
            putsArray(iB, " => 1st contiguous sum to " + sum + ": [" + naive[0] + ", " + naive[1] + "] (naive algo)");
            int range[] = firstContiguousSumToN(iB, sum);
            putsArray(iB, " => 1st contiguous sum to " + sum + ": [" + range[0] + ", " + range[1] + "]");
        }

        int stat = test_maxContiguousSubArray();

        return stat;
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
};

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