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
  std::array<float, 4> arr1{1, 2, 3, 4};
  std::array<float, 4> arr2{5, 6, 7, 8};
  Matrix<2, 2> mat1{arr1};
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
  }

  SECTION("Subtraction") {
    mat1.Sub(mat2, mat3);

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

    // try a non-square matrix
  }

  SECTION("Scalar Multiplication") {
    mat1.Mult(2, mat3);

    REQUIRE(mat3.Get(0, 0) == 2);
    REQUIRE(mat3.Get(0, 1) == 4);
    REQUIRE(mat3.Get(1, 0) == 6);
    REQUIRE(mat3.Get(1, 1) == 8);
  }

  SECTION("Squaring") {
    mat1.Square(mat3);

    REQUIRE(mat3.Get(0, 0) == 7);
    REQUIRE(mat3.Get(0, 1) == 10);
    REQUIRE(mat3.Get(1, 0) == 15);
    REQUIRE(mat3.Get(1, 1) == 22);
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

    REQUIRE(mat3.Get(0, 0) == 1 / 5);
    REQUIRE(mat3.Get(0, 1) == 2 / 6);
    REQUIRE(mat3.Get(1, 0) == 3 / 7);
    REQUIRE(mat3.Get(1, 1) == 4 / 8);
  }

  SECTION("Determinant") {
    float det1 = mat1.Det();

    REQUIRE_THAT(det1, Catch::Matchers::WithinRel(-2.0F, 1e-6f));

    std::array<float, 9> arr4{1, 2, 3, 4, 5, 6, 7, 8, 9};
    Matrix<3, 3> mat4{arr4};

    float det4 = mat4.Det();

    REQUIRE_THAT(det4, Catch::Matchers::WithinRel(0.0F, 1e-6f));

    std::array<float, 9> arr5{1, 0, 0, 0, 2, 0, 0, 0, 3};
    Matrix<3, 3> mat5{arr5};

    float det5 = mat5.Det();
    REQUIRE_THAT(det5, Catch::Matchers::WithinRel(6.0F, 1e-6f));
  }

  SECTION("Invert"){};

  SECTION("Transpose") {
    // transpose a square matrix
    mat1.Transpose(mat3);

    REQUIRE(mat3.Get(0, 0) == 1);
    REQUIRE(mat3.Get(0, 1) == 3);
    REQUIRE(mat3.Get(1, 0) == 2);
    REQUIRE(mat3.Get(1, 1) == 4);

    // transpose a non-square matrix
    std::array<float, 6> arr4{1, 2, 3, 4, 5, 6};
    Matrix<2, 3> mat4{arr4};
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

    std::array<float, 2> arr4{-0.878877044, 2.92092276};
    Matrix<2, 1> mat4{arr4};
    Matrix<2, 1> mat5{};
    mat4.Normalize(mat5);

    REQUIRE_THAT(mat5.Get(0, 0),
                 Catch::Matchers::WithinRel(-0.288129855179f, 1e-6f));
    REQUIRE_THAT(mat5.Get(1, 0),
                 Catch::Matchers::WithinRel(0.957591346325f, 1e-6f));
  }

  SECTION("GET ROW") {}

  SECTION("GET COLUMN") {}
}