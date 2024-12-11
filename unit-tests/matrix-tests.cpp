// include the unit test framework first
#include <catch2/catch_test_macros.hpp>

// include the module you're going to test next
#include "Matrix.hpp"

// any other libraries
#include <array>
#include <iostream>

TEST_CASE("Elementary Matrix Operations", "Matrix::Add") {
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
  }

  SECTION("Scalar Multiplication") {
    mat1.Mult(2, mat3);

    REQUIRE(mat3.Get(0, 0) == 2);
    REQUIRE(mat3.Get(0, 1) == 4);
    REQUIRE(mat3.Get(1, 0) == 6);
    REQUIRE(mat3.Get(1, 1) == 8);
  }
}