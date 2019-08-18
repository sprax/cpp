/// @file  include/matrix/matrix.h
#pragma once

#include <vector>


/** STL vector-of-vectors-based matrix */
template <typename T>
class VVMatrix {

    template<typename U>
    using vector = std::vector<U>;      // private templated typedef

    public:

    // Parameter Constructor
    VVMatrix(unsigned rows_, unsigned cols_, const T& value)
            : rows(rows_), cols(cols_)
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
