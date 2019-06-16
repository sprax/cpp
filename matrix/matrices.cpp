// Compile and Run:
// Mac osx:
/*  GCC g++ vs. CLANG clang++ (both supply C++ library linkage):
        g++ hello.cpp -o hello.out && hello.out  	# [you might need ./hello.out]
        clang++ hello.cpp -o hello.out && hello.out  	# [you might need ./hello.out]
    GCC and CLANG front ends, with explicit C++ library link options:
        gcc -lc++ hello.cpp -o hello.out && hello.out
        clang -lc++ hello.cpp -o hello.out && hello.out (default C++ library)
        clang -lstdc++ hello.cpp -o hello.out && hello.out (Use libstdc++ for compatibility with libMLi3)
    CC is a symbolic link to clang:  /usr/bin/CC@ -> clang
   Windows 10: very similar:
   	g++ hello.cpp -o hello.exe && hello
*/
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/** STL vector-of-vectors-based matrix */
template <typename T>
class VVMatrix {
    public:

    // Parameter Constructor
    VVMatrix(unsigned rows_, unsigned cols_, const T& value) : rows(rows_), cols(cols_)
    {
        vecs.resize(rows);
        for (unsigned i = 0; i < vecs.size(); i++) {
            vecs[i].resize(cols, value);
        }
    }

    VVMatrix(const VVMatrix<T>& rhs) {      // Copy Ctor
        vecs = rhs.vecs;
        rows = rhs.getRows();
        cols = rhs.getCols();
    }

    ~VVMatrix() {}        // Virtual Dtor

    vector<T> &operator[] (unsigned idx) {
        return vecs.at(idx);
    }

    inline double& operator()(int x, int y) { return vecs[x][y]; }

    unsigned getRows() { return rows; }
    unsigned getCols() { return cols; }

private:
    vector<vector<T> > vecs;  // row vectors
    unsigned rows;
    unsigned cols;
};


/** Native new-allocated matrix of doubles */
class N_Matrix {
    public:
        N_Matrix(int rows_, int cols_) : rows(rows_), cols(cols_)   // Parameter ctor
        { allocSpace(); }

        N_Matrix() : rows(1), cols(1)                               // Default ctor
        {
          allocSpace();
          **pRows = 0.0;
        }

        N_Matrix(const N_Matrix& src) : rows(src.rows), cols(src.cols)  // Copy ctor
        {
            allocSpace();
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    pRows[i][j] = src.pRows[i][j];
                }
            }
        }

        virtual ~N_Matrix()                                           // Virtual Dtor
        {
            deallocSpace();
        }

        N_Matrix& operator=(const N_Matrix& rhs)
        {
            if (this == &rhs) {
                return *this;
            }

            if (rows != rhs.rows || cols != rhs.cols) {
                deallocSpace();
                rows = rhs.rows;
                cols = rhs.cols;
                allocSpace();
            }

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    pRows[i][j] = rhs.pRows[i][j];
                }
            }
            return *this;
        }

        inline double& operator()(int x, int y) { return pRows[x][y]; }

    private:
        void allocSpace()
        {
            pRows = new double*[rows];
            for (int i = 0; i < rows; ++i) {
                pRows[i] = new double[cols];
            }
        }

        void deallocSpace()
        {
            for (int i = 0; i < rows; ++i) {
                delete[] pRows[i];
            }
            delete[] pRows;
        }


        int rows, cols;
        double **pRows;
};


class MM_Matrix {
    public:
        MM_Matrix(int, int);
        MM_Matrix();
        ~MM_Matrix();
        MM_Matrix(const MM_Matrix&);
        MM_Matrix& operator=(const MM_Matrix&);

        inline double& operator()(int x, int y) { return pRows_[x][y]; }

    private:
        void allocSpace()
        {
          pMem_ = new double*[rows*cols];
          pRows_ = new double*[rows];
            for (int i = 0; i < rows; ++i) {
                pRows_[i] = new double[cols];
            }
        }

        int rows;
        int cols;
        int elt_size_;
        int row_size_;  // stride
        int mem_size_;  // rows * cols* * elt_size_ unless padding or border

        double **pMem_;
        double **pRows_;
};



int main(int argc, char* argv[]) {
    cout << argv[0] << ":" << endl;

    VVMatrix<double> vvmA(2, 4, 5.0);
    cout << "vvmA: " << vvmA.getRows() << " x " << vvmA.getCols() << " value: "
         << vvmA(0, 0) << " == " << vvmA[0][0] << endl;
    VVMatrix<double> vvmB(3, 5, 6.0);
    cout << "Reassigning: vvmA = vvmB:" << endl;
    vvmA = vvmB;
    cout << "vvmA: " << vvmA.getRows() << " x " << vvmA.getCols() << " value: "
         << vvmA(0, 0) << " == " << vvmA[0][0] << endl;

    std::vector<double> vec = {1.1, 2.2, 3.3};
    std::cout << "vec * 1: " << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
    for (double& dd : vec) {
         dd *= 2;
    }
    std::cout << "vec * 2: " << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
    return 0;
}
