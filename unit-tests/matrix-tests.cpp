// include the unit test framework first
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// include the module you're going to test next
#include "Matrix.hpp"

// any other libraries
#include <array>
#include <cmath>
#include <iostream>

TEST_CASE("Elementary Matrix Operations", "Matrix") {
  std::array<float, 4> arr2{5, 6, 7, 8};
  Matrix<2, 2> mat1{1, 2, 3, 4};
  Matrix<2, 2> mat2{arr2};
  Matrix<2, 2> mat3{};

  SECTION("Initialization") {
    // array initialization
    REQUIRE(mat1.Get(0, 0) == 1);
    REQUIRE(mat1.Get(0, 1) == 2);
    REQUIRE(mat1.Get(1, 0) == 3);
    REQUIRE(mat1.Get(1, 1) == 4);

    // empty initialization
    REQUIRE(mat3.Get(0, 0) == 0);
    REQUIRE(mat3.Get(0, 1) == 0);
    REQUIRE(mat3.Get(1, 0) == 0);
    REQUIRE(mat3.Get(1, 1) == 0);

    // template pack initialization
    REQUIRE(mat2.Get(0, 0) == 5);
    REQUIRE(mat2.Get(0, 1) == 6);
    REQUIRE(mat2.Get(1, 0) == 7);
    REQUIRE(mat2.Get(1, 1) == 8);

    // large matrix
    Matrix<255, 255> mat6{};
    mat6.Fill(4);
    for (uint8_t row{0}; row < 255; row++) {
      for (uint8_t column{0}; column < 255; column++) {
        REQUIRE(mat6.Get(row, column) == 4);
      }
    }
  }

  SECTION("Fill") {
    mat1.Fill(0);
    REQUIRE(mat1.Get(0, 0) == 0);
    REQUIRE(mat1.Get(0, 1) == 0);
    REQUIRE(mat1.Get(1, 0) == 0);
    REQUIRE(mat1.Get(1, 1) == 0);

    mat2.Fill(100000);
    REQUIRE(mat2.Get(0, 0) == 100000);
    REQUIRE(mat2.Get(0, 1) == 100000);
    REQUIRE(mat2.Get(1, 0) == 100000);
    REQUIRE(mat2.Get(1, 1) == 100000);

    mat3.Fill(-20);
    REQUIRE(mat3.Get(0, 0) == -20);
    REQUIRE(mat3.Get(0, 1) == -20);
    REQUIRE(mat3.Get(1, 0) == -20);
    REQUIRE(mat3.Get(1, 1) == -20);
  }

  SECTION("Addition") {
    std::string strBuf1 = "";
    mat1.ToString(strBuf1);
    std::cout << "Matrix 1:\n" << strBuf1 << std::endl;

    mat1.Add(mat2, mat3);

    REQUIRE(mat3.Get(0, 0) == 6);
    REQUIRE(mat3.Get(0, 1) == 8);
    REQUIRE(mat3.Get(1, 0) == 10);
    REQUIRE(mat3.Get(1, 1) == 12);

    // try out addition with overloaded operators
    mat3.Fill(0);
    mat3 = mat1 + mat2;
    REQUIRE(mat3.Get(0, 0) == 6);
    REQUIRE(mat3.Get(0, 1) == 8);
    REQUIRE(mat3.Get(1, 0) == 10);
    REQUIRE(mat3.Get(1, 1) == 12);
  }

  SECTION("Subtraction") {
    mat1.Sub(mat2, mat3);

    REQUIRE(mat3.Get(0, 0) == -4);
    REQUIRE(mat3.Get(0, 1) == -4);
    REQUIRE(mat3.Get(1, 0) == -4);
    REQUIRE(mat3.Get(1, 1) == -4);

    // try out subtraction with operators
    mat3.Fill(0);
    mat3 = mat1 - mat2;
    REQUIRE(mat3.Get(0, 0) == -4);
    REQUIRE(mat3.Get(0, 1) == -4);
    REQUIRE(mat3.Get(1, 0) == -4);
    REQUIRE(mat3.Get(1, 1) == -4);
  }

  SECTION("Multiplication") {
    mat1.Mult(mat2, mat3);

    REQUIRE(mat3.Get(0, 0) == 19);
    REQUIRE(mat3.Get(0, 1) == 22);
    REQUIRE(mat3.Get(1, 0) == 43);
    REQUIRE(mat3.Get(1, 1) == 50);

    // try out multiplication with operators
    mat3.Fill(0);
    mat3 = mat1 * mat2;
    REQUIRE(mat3.Get(0, 0) == 19);
    REQUIRE(mat3.Get(0, 1) == 22);
    REQUIRE(mat3.Get(1, 0) == 43);
    REQUIRE(mat3.Get(1, 1) == 50);
  }

  SECTION("Scalar Multiplication") {
    mat1.Mult(2, mat3);

    REQUIRE(mat3.Get(0, 0) == 2);
    REQUIRE(mat3.Get(0, 1) == 4);
    REQUIRE(mat3.Get(1, 0) == 6);
    REQUIRE(mat3.Get(1, 1) == 8);
  }

  SECTION("Element Multiply") {
    mat1.ElementMultiply(mat2, mat3);

    REQUIRE(mat3.Get(0, 0) == 5);
    REQUIRE(mat3.Get(0, 1) == 12);
    REQUIRE(mat3.Get(1, 0) == 21);
    REQUIRE(mat3.Get(1, 1) == 32);
  }

  SECTION("Element Divide") {
    mat1.ElementDivide(mat2, mat3);

    REQUIRE_THAT(mat3.Get(0, 0), Catch::Matchers::WithinRel(0.2f, 1e-6f));
    REQUIRE_THAT(mat3.Get(0, 1), Catch::Matchers::WithinRel(0.3333333f, 1e-6f));
    REQUIRE_THAT(mat3.Get(1, 0), Catch::Matchers::WithinRel(0.4285714f, 1e-6f));
    REQUIRE_THAT(mat3.Get(1, 1), Catch::Matchers::WithinRel(0.5f, 1e-6f));
  }

  SECTION("Minor Matrix") {
    // what about matrices of 0,0 or 1,1?
    // minor matrix for 2x2 matrix
    Matrix<1, 1> minorMat1{};
    mat1.MinorMatrix(minorMat1, 0, 0);
    REQUIRE(minorMat1.Get(0, 0) == 4);
    mat1.MinorMatrix(minorMat1, 0, 1);
    REQUIRE(minorMat1.Get(0, 0) == 3);
    mat1.MinorMatrix(minorMat1, 1, 0);
    REQUIRE(minorMat1.Get(0, 0) == 2);
    mat1.MinorMatrix(minorMat1, 1, 1);
    REQUIRE(minorMat1.Get(0, 0) == 1);

    // minor matrix for 3x3 matrix
    Matrix<3, 3> mat4{1, 2, 3, 4, 5, 6, 7, 8, 9};
    Matrix<2, 2> minorMat4{};

    mat4.MinorMatrix(minorMat4, 0, 0);
    REQUIRE(minorMat4.Get(0, 0) == 5);
    REQUIRE(minorMat4.Get(0, 1) == 6);
    REQUIRE(minorMat4.Get(1, 0) == 8);
    REQUIRE(minorMat4.Get(1, 1) == 9);

    mat4.MinorMatrix(minorMat4, 1, 1);
    REQUIRE(minorMat4.Get(0, 0) == 1);
    REQUIRE(minorMat4.Get(0, 1) == 3);
    REQUIRE(minorMat4.Get(1, 0) == 7);
    REQUIRE(minorMat4.Get(1, 1) == 9);

    mat4.MinorMatrix(minorMat4, 2, 2);
    REQUIRE(minorMat4.Get(0, 0) == 1);
    REQUIRE(minorMat4.Get(0, 1) == 2);
    REQUIRE(minorMat4.Get(1, 0) == 4);
    REQUIRE(minorMat4.Get(1, 1) == 5);
  }

  SECTION("Determinant") {
    float det1 = mat1.Det();

    REQUIRE_THAT(det1, Catch::Matchers::WithinRel(-2.0F, 1e-6f));

    Matrix<3, 3> mat4{1, 2, 3, 4, 5, 6, 7, 8, 9};

    float det4 = mat4.Det();

    REQUIRE_THAT(det4, Catch::Matchers::WithinRel(0.0F, 1e-6f));

    Matrix<3, 3> mat5{1, 0, 0, 0, 2, 0, 0, 0, 3};

    float det5 = mat5.Det();
    REQUIRE_THAT(det5, Catch::Matchers::WithinRel(6.0F, 1e-6f));
  }

  SECTION("Matrix of Minors") {
    mat1.MatrixOfMinors(mat3);
    REQUIRE_THAT(mat3.Get(0, 0), Catch::Matchers::WithinRel(4.0F, 1e-6f));
    REQUIRE_THAT(mat3.Get(0, 1), Catch::Matchers::WithinRel(3.0F, 1e-6f));
    REQUIRE_THAT(mat3.Get(1, 0), Catch::Matchers::WithinRel(2.0F, 1e-6f));
    REQUIRE_THAT(mat3.Get(1, 1), Catch::Matchers::WithinRel(1.0F, 1e-6f));

    Matrix<3, 3> mat4{1, 2, 3, 4, 5, 6, 7, 8, 9};
    Matrix<3, 3> mat5{0};
    mat4.MatrixOfMinors(mat5);
    REQUIRE_THAT(mat5.Get(0, 0), Catch::Matchers::WithinRel(-3.0F, 1e-6f));
    REQUIRE_THAT(mat5.Get(0, 1), Catch::Matchers::WithinRel(-6.0F, 1e-6f));
    REQUIRE_THAT(mat5.Get(0, 2), Catch::Matchers::WithinRel(-3.0F, 1e-6f));
    REQUIRE_THAT(mat5.Get(1, 0), Catch::Matchers::WithinRel(-6.0F, 1e-6f));
    REQUIRE_THAT(mat5.Get(1, 1), Catch::Matchers::WithinRel(-12.0F, 1e-6f));
    REQUIRE_THAT(mat5.Get(1, 2), Catch::Matchers::WithinRel(-6.0F, 1e-6f));
    REQUIRE_THAT(mat5.Get(2, 0), Catch::Matchers::WithinRel(-3.0F, 1e-6f));
    REQUIRE_THAT(mat5.Get(2, 1), Catch::Matchers::WithinRel(-6.0F, 1e-6f));
    REQUIRE_THAT(mat5.Get(2, 2), Catch::Matchers::WithinRel(-3.0F, 1e-6f));
  }

  SECTION("Invert") {
    mat1.Invert(mat3);
    REQUIRE_THAT(mat3.Get(0, 0), Catch::Matchers::WithinRel(-2.0F, 1e-6f));
    REQUIRE_THAT(mat3.Get(0, 1), Catch::Matchers::WithinRel(1.0F, 1e-6f));
    REQUIRE_THAT(mat3.Get(1, 0), Catch::Matchers::WithinRel(1.5F, 1e-6f));
    REQUIRE_THAT(mat3.Get(1, 1), Catch::Matchers::WithinRel(-0.5F, 1e-6f));
  };

  SECTION("Transpose") {
    // transpose a square matrix
    mat1.Transpose(mat3);

    REQUIRE(mat3.Get(0, 0) == 1);
    REQUIRE(mat3.Get(0, 1) == 3);
    REQUIRE(mat3.Get(1, 0) == 2);
    REQUIRE(mat3.Get(1, 1) == 4);

    // transpose a non-square matrix
    Matrix<2, 3> mat4{1, 2, 3, 4, 5, 6};
    Matrix<3, 2> mat5{};

    mat4.Transpose(mat5);

    REQUIRE(mat5.Get(0, 0) == 1);
    REQUIRE(mat5.Get(0, 1) == 4);
    REQUIRE(mat5.Get(1, 0) == 2);
    REQUIRE(mat5.Get(1, 1) == 5);
    REQUIRE(mat5.Get(2, 0) == 3);
    REQUIRE(mat5.Get(2, 1) == 6);
  }

  SECTION("Normalize") {
    mat1.Normalize(mat3);

    float sqrt_30{sqrt(30)};

    REQUIRE(mat3.Get(0, 0) == 1 / sqrt_30);
    REQUIRE(mat3.Get(0, 1) == 2 / sqrt_30);
    REQUIRE(mat3.Get(1, 0) == 3 / sqrt_30);
    REQUIRE(mat3.Get(1, 1) == 4 / sqrt_30);

    Matrix<2, 1> mat4{-0.878877044, 2.92092276};
    Matrix<2, 1> mat5{};
    mat4.Normalize(mat5);

    REQUIRE_THAT(mat5.Get(0, 0),
                 Catch::Matchers::WithinRel(-0.288129855179f, 1e-6f));
    REQUIRE_THAT(mat5.Get(1, 0),
                 Catch::Matchers::WithinRel(0.957591346325f, 1e-6f));
  }

  SECTION("GET ROW") {
    Matrix<1, 2> mat1Rows{};
    mat1.GetRow(0, mat1Rows);
    REQUIRE(mat1Rows.Get(0, 0) == 1);
    REQUIRE(mat1Rows.Get(0, 1) == 2);

    mat1.GetRow(1, mat1Rows);
    REQUIRE(mat1Rows.Get(0, 0) == 3);
    REQUIRE(mat1Rows.Get(0, 1) == 4);
  }

  SECTION("GET COLUMN") {
    Matrix<2, 1> mat1Columns{};
    mat1.GetColumn(0, mat1Columns);
    REQUIRE(mat1Columns.Get(0, 0) == 1);
    REQUIRE(mat1Columns.Get(1, 0) == 3);

    mat1.GetColumn(1, mat1Columns);
    REQUIRE(mat1Columns.Get(0, 0) == 2);
    REQUIRE(mat1Columns.Get(1, 0) == 4);
  }
}

// basically re-run all of the previous tests with huge matrices and time the
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
      mat4.Invert(mat5);
    }
  };

  SECTION("Transpose") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat1.Transpose(mat3);
    }
  }

  SECTION("Normalize") {
    for (uint32_t i{0}; i < 10000; i++) {
      mat1.Normalize(mat3);
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