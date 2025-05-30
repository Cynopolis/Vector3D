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

    // TODO: You need to add non-square multiplications to this.
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
    mat3 = mat1.Invert();
    REQUIRE_THAT(mat3.Get(0, 0), Catch::Matchers::WithinRel(-2.0F, 1e-6f));
    REQUIRE_THAT(mat3.Get(0, 1), Catch::Matchers::WithinRel(1.0F, 1e-6f));
    REQUIRE_THAT(mat3.Get(1, 0), Catch::Matchers::WithinRel(1.5F, 1e-6f));
    REQUIRE_THAT(mat3.Get(1, 1), Catch::Matchers::WithinRel(-0.5F, 1e-6f));
  };

  SECTION("Transpose") {
    // transpose a square matrix
    mat3 = mat1.Transpose();

    REQUIRE(mat3.Get(0, 0) == 1);
    REQUIRE(mat3.Get(0, 1) == 3);
    REQUIRE(mat3.Get(1, 0) == 2);
    REQUIRE(mat3.Get(1, 1) == 4);

    // transpose a non-square matrix
    Matrix<2, 3> mat4{1, 2, 3, 4, 5, 6};
    Matrix<3, 2> mat5{};

    mat5 = mat4.Transpose();

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

  SECTION("Get Sub-Matrices") {
    Matrix<3, 3> mat4{1, 2, 3, 4, 5, 6, 7, 8, 9};

    Matrix<2, 2> mat5 = mat4.SubMatrix<2, 2, 0, 0>();

    REQUIRE(mat5.Get(0, 0) == 1);
    REQUIRE(mat5.Get(0, 1) == 2);
    REQUIRE(mat5.Get(1, 0) == 4);
    REQUIRE(mat5.Get(1, 1) == 5);

    mat5 = mat4.SubMatrix<2, 2, 1, 1>();
    REQUIRE(mat5.Get(0, 0) == 5);
    REQUIRE(mat5.Get(0, 1) == 6);
    REQUIRE(mat5.Get(1, 0) == 8);
    REQUIRE(mat5.Get(1, 1) == 9);

    Matrix<3, 1> mat6 = mat4.SubMatrix<3, 1, 0, 0>();
    REQUIRE(mat6.Get(0, 0) == 1);
    REQUIRE(mat6.Get(1, 0) == 4);
    REQUIRE(mat6.Get(2, 0) == 7);

    Matrix<1, 3> mat7 = mat4.SubMatrix<1, 3, 0, 0>();
    REQUIRE(mat7.Get(0, 0) == 1);
    REQUIRE(mat7.Get(0, 1) == 2);
    REQUIRE(mat7.Get(0, 2) == 3);
  }

  SECTION("Set Sub-Matrices") {
    Matrix<3, 3> startMatrix{1, 2, 3, 4, 5, 6, 7, 8, 9};
    Matrix<3, 3> mat4 = startMatrix;

    Matrix<2, 2> mat5{10, 11, 12, 13};
    mat4.SetSubMatrix<2, 2, 0, 0>(mat5);
    REQUIRE(mat4.Get(0, 0) == 10);
    REQUIRE(mat4.Get(0, 1) == 11);
    REQUIRE(mat4.Get(1, 0) == 12);
    REQUIRE(mat4.Get(1, 1) == 13);

    mat4 = startMatrix;
    mat4.SetSubMatrix<2, 2, 1, 1>(mat5);
    REQUIRE(mat4.Get(1, 1) == 10);
    REQUIRE(mat4.Get(1, 2) == 11);
    REQUIRE(mat4.Get(2, 1) == 12);
    REQUIRE(mat4.Get(2, 2) == 13);

    Matrix<3, 1> mat6{10, 11, 12};
    mat4.SetSubMatrix<3, 1, 0, 0>(mat6);
    REQUIRE(mat4.Get(0, 0) == 10);
    REQUIRE(mat4.Get(1, 0) == 11);
    REQUIRE(mat4.Get(2, 0) == 12);

    Matrix<1, 3> mat7{10, 11, 12};
    mat4.SetSubMatrix<1, 3, 0, 0>(mat7);
    REQUIRE(mat4.Get(0, 0) == 10);
    REQUIRE(mat4.Get(0, 1) == 11);
    REQUIRE(mat4.Get(0, 2) == 12);
  }
}

TEST_CASE("Advanced Matrix Operations", "Matrix") {
  SECTION("2x2 QRDecomposition") {
    Matrix<2, 2> A{1.0f, 2.0f, 3.0f, 4.0f};
    Matrix<2, 2> Q{}, R{};
    A.QRDecomposition(Q, R);

    // Check that Q * R ≈ A
    Matrix<2, 2> QR{};
    Q.Mult(R, QR);
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
        REQUIRE_THAT(QR[i][j], Catch::Matchers::WithinRel(A[i][j], 1e-4f));
      }
    }

    // Check that Q is orthonormal: Qᵀ * Q ≈ I
    Matrix<2, 2> Qt = Q.Transpose();
    Matrix<2, 2> QtQ{};
    Qt.Mult(Q, QtQ);
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
        if (i == j)
          REQUIRE_THAT(QtQ[i][j], Catch::Matchers::WithinRel(1.0f, 1e-4f));
        else
          REQUIRE_THAT(QtQ[i][j], Catch::Matchers::WithinAbs(0.0f, 1e-4f));
      }
    }

    // Optional: R should be upper triangular
    REQUIRE(std::fabs(R[1][0]) < 1e-4f);
  }

  SECTION("3x3 QRDecomposition") {
    // this symmetrix tridiagonal matrix is well behaved for testing
    Matrix<3, 3> A{3.0f, -1.0f, 0.0f, -1.0f, 3.0f, -1.0f, 0.0f, -1.0f, 3.0f};

    Matrix<3, 3> Q{}, R{};
    A.QRDecomposition(Q, R);

    // Check that Q * R ≈ A
    Matrix<3, 3> QR{};
    Q.Mult(R, QR);
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        REQUIRE_THAT(QR[i][j], Catch::Matchers::WithinRel(A[i][j], 1e-4f));
      }
    }

    // Check that Qᵀ * Q ≈ I
    Matrix<3, 3> Qt = Q.Transpose();
    Matrix<3, 3> QtQ{};
    Qt.Mult(Q, QtQ);
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (i == j)
          REQUIRE_THAT(QtQ[i][j], Catch::Matchers::WithinRel(1.0f, 1e-4f));
        else
          REQUIRE_THAT(QtQ[i][j], Catch::Matchers::WithinAbs(0.0f, 1e-4f));
      }
    }

    // Optional: Check R is upper triangular
    for (int i = 1; i < 3; ++i) {
      for (int j = 0; j < i; ++j) {
        REQUIRE(std::fabs(R[i][j]) < 1e-4f);
      }
    }
  }
}