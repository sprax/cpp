// Sx : convenience class (namespace) for static output functions
// Sprax Lines 2016 
// Based on Sx.java.  Uses printf, not cout.
// Sx.cs exists as well.

#ifndef Sx_h
#define Sx_h

class Sx
{
public:

    //// print and println as in Java ////

    inline static void print(char *c_str) {
        printf("%s", c_str);
    }

    inline static void print(std::string str) {
        printf("%s", str.c_str());
    }

    inline static void println(char *c_str) {
        puts(c_str);
    }

    inline static void println(std::string str) {
        puts(str.c_str());
    }

    inline static void println(void) {
        puts("");
    }

    //// print one item with spacing as if in an array ////

    inline static void printOne(char val) {
        printf(" %c", val);
    }

    inline static void printOne(int val) {
        printf(" %2d", val);
    }

    inline static void printOne(double val) {
        printf(" %2.2f", val);
    }

    inline static void printOne(std::string str) {
        printf(" %s", str.c_str());
    }

    //// print a 1-dimensional array ////

    template <typename T>
    static void printArray(T A[], size_t length) {
        for (size_t j = 0; j < length; j++)
            printOne(A[j]);
    }

    template <typename T>
    static void putsArray(T A[], int length)
    {
        printArray(A, length);
        puts("");
    }

    template <typename T>
    static void printSubArray(T A[], int first, int last, bool indent = false)
    {
        if (indent) {
            for (int j = 0; j < first; j++)
                printf("  ");
        }
        for (int j = first; j < last; j++)
            printOne(A[j]);
    }

    template <typename T>
    static void putsSubArray(T A[], int first, int last, bool indent = false)
    {
        printSubArray(A, first, last);
        puts("\n");
    }

    template <typename T>
    static void putsArray(const char *preLabel, T A[], int length)
    {
        printf(preLabel);
        putsArray(A, length);
    }

    template <typename T>
    static void putsArray(const char *preLabel, T A[], int length, const char *postLabel)
    {
        printf(preLabel);
        putsArray(A, length, postLabel);
    }

    //// print a 2-dimensional array ////

    /**
    * print a 2d array passed by reference.
    * This may work only for automatic arrays (usually abiding on the stack)
    * whose dimensions are known at compile time (and thus can be used
    * to evaluate the Rows and Cols template arguments).
    * For dynamic (freestore) arrays on the heap, it may not compile.
    */
    template <typename T, size_t Rows, size_t Cols>
    static void putsArrayByRef2(const T(&array)[Rows][Cols])
    {
        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                printOne(array[row][col]);
            }
            puts("");
        }
        puts("");
    }

    /**
    * print a 2d array passed by as a pointer to the array of templatized width (number
    * of columns per row).
    * This may work only for automatic arrays or an array of pointers to automatic
    * arrays (one for each row) where the width (number of columns) is known at compile time.
    */
    template <typename T, size_t Cols>
    static void putsArrayByPtrRef2(const T(*array)[][Cols], size_t rows)
    {
        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                printOne((*array)[row][col]);
            }
            puts("");
        }
        puts("");
    }

    /**
    * print a 2d array by passing (by value) a pointer to the decayed type
    * of the array, which is a pointer to a variable number of rows of known
    * constant width.  This width must be a constant specified in the original
    * declaration of the calling argument array, or in a cast of a pointer to
    * that array (so that one way or another, the width is specified at compile time).
    * Since the argument type does not specified the number of rows, this number
    * must be passed in to the method as a separate argument.  The compiler can
    * only check that the number of columns matches that specified in the
    * second dimension of the argument type.
    */
    template <typename T>
    static void putsArrayByPtr2(const T(*array)[], size_t rows, size_t cols)
    {
        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < cols; col++) {
                printOne((*array)[row][col]);
            }
            puts("");
        }
        puts("");
    }

    /**
    * print a 2d array by passing a pointer to a pointer, taking advantage
    * of the decay of arrays into pointers (which C++ "inherited" from C,
    * i.e. allows for backward compatibility with C).  This method is not
    * safe for actual 2d arrays, but is guaranteed to work with a 1D array
    * of pointers to 1D arrays (the rows as separate arrays).  Due to array
    * flattening or memory padding/alignment, the offsets to each row might
    * not be the expected multiple of the number of columns.  That is, while
    * some compilers might let you get away with casting a 2D array to a
    * pointer to a pointer (as in (const T **)&array[0][0]), the only portably
    * safe way to call this method is to define an auxiliary array of pointers
    * to each rows (as in T *X[Rows]; for (int r = 0; r < Rows; r++) X[r] = A[r];),
    * and then to pass this auxiliary array as the argument to the method.
    * See the tests below.
    */
    template <typename T>
    static void putsArrayByPtrPtr2(const T *array[], size_t rows, size_t cols)
    {
        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < cols; col++) {
                printOne(array[row][col]);
            }
            puts("");
        }
        puts("");
    }

    static int unit_test();

};

#endif // Sx_h