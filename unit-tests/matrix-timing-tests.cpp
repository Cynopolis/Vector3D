// include the unit test framework first
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// include the module you're going to test next
#include "Matrix.hpp"

// any other libraries
#include <array>
#include <cmath>

// basically re-run all of the matrix tests with huge matrices and time the
// results.
TEST_CASE("Timing Tests", "Matrix") {
  std::array<float, 50 * 50> arr1{};
  for (uint16_t i{0}; i < 50 * 50; i++) {
    arr1[i] = i;
  }
  std::array<float, 50 * 50> arr2{5, 6, 7, 8};
  for (uint16_t i{50 * 50}; i < 2 * 50 * 50; i++) {
    arr2[i] = i;
  }
  Matrix<50, 50> mat1{arr1};
  Matrix<50, 50> mat2{arr2};
  Matrix<50, 50> mat3{};

  // A smaller matrix to use for really badly optimized operations
  Matrix<4, 4> mat4{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  Matrix<4, 4> mat5{};

  SECTION("Addition") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat3 = mat1 + mat2;
    }
  }

  SECTION("Subtraction") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat3 = mat1 - mat2;
    }
  }

  SECTION("Multiplication") {
    for (uint32_t i{0}; i < 1000; i++) {
      mat3 = mat1 * mat2;
    }
  }

  SECTION("Scalar Multiplication") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat3 = mat1 * 3;
    }
  }

  SECTION("Element Multiply") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat1.ElementMultiply(mat2, mat3);
    }
  }

  SECTION("Element Divide") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat1.ElementDivide(mat2, mat3);
    }
  }

  SECTION("Minor Matrix") {
    // what about matrices of 0,0 or 1,1?
    // minor matrix for 2x2 matrix
    Matrix<49, 49> minorMat1{};
    for (uint32_t i{0}; i < 10000; i++) {
      mat1.MinorMatrix(minorMat1, 0, 0);
    }
  }

  SECTION("Determinant") {
    for (uint32_t i{0}; i < 100000; i++) {
      float det1 = mat4.Det();
    }
  }

  SECTION("Matrix of Minors") {
    for (uint32_t i{0}; i < 100000; i++) {
      mat4.MatrixOfMinors(mat5);
    }
  }

  SECTION("Invert") {
    for (uint32_t i{0}; i < 100000; i++) {
      mat5 = mat4.Invert();
    }
  };

  SECTION("Transpose") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat3 = mat1.Transpose();
    }
  }

  SECTION("Normalize") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat3 = mat1 / mat1.EuclideanNorm();
    }
  }

  SECTION("GET ROW") {
    Matrix<1, 50> mat1Rows{};
    for (uint32_t i{0}; i < 1000000; i++) {
      mat1.GetRow(0, mat1Rows);
    }
  }

  SECTION("GET COLUMN") {
    Matrix<50, 1> mat1Columns{};
    for (uint32_t i{0}; i < 1000000; i++) {
      mat1.GetColumn(0, mat1Columns);
    }
  }
}