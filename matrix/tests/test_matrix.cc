// @file  test_matrix.cc - part of a googletest suite
#include "matrix/matrix.h"

#include <gtest/gtest.h>
#include <iostream>

using std::cout;
using std::endl;
// using namespace std;

TEST(Matrix, Make)
{
    VVMatrix<double> vvmA(2, 4, 5.0);
    cout<< "vvmA: " << vvmA.getRows() << " x " << vvmA.getCols() << " value: "
        << vvmA(0, 0) << " == " << vvmA[0][0] << endl;
    EXPECT_EQ( vvmA.getRows(), 2 );
    EXPECT_EQ( vvmA.getCols(), 4 );

    VVMatrix<double> vvmB(3, 5, 6.0);
    EXPECT_EQ( vvmB.getRows(), 3 );
    EXPECT_EQ( vvmB.getCols(), 5 );

    cout << "Reassigning: vvmA = vvmB:" << endl;
    vvmA = vvmB;
    cout<< "vvmA: " << vvmA.getRows() << " x " << vvmA.getCols() << " value: "
        << vvmA(0, 0) << " == " << vvmA[0][0] << endl;
    EXPECT_EQ( vvmA.getRows(), 3 );
    EXPECT_EQ( vvmA.getCols(), 5 );

    std::vector<double> vec = {1.1, 2.2, 3.3};
    std::cout << "vec * 1: " << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;
    for (double& dd : vec) {
         dd *= 2;
    }
    std::cout << "vec * 2: " << vec[0] << ", " << vec[1] << ", " << vec[2] << std::endl;

    EXPECT_EQ( vec[0], 2.2 );
    EXPECT_EQ( vec[1], 4.4 );
    EXPECT_EQ( vec[2], 6.6 );
}
