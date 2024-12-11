// include the unit test framework first
#include <catch2/catch_test_macros.hpp>

// include the module you're going to test next
#include "Matrix.hpp"

// any other libraries
#include <array>

unsigned int Factorial(unsigned int number) {
  return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
  REQUIRE(Factorial(1) == 1);
  REQUIRE(Factorial(2) == 2);
  REQUIRE(Factorial(3) == 6);
  REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("Matrix Addition", "Matrix::Add") {
  std::array<float, 4> arr1{1, 2, 3, 4};
  std::array<float, 4> arr2{5, 6, 7, 8};
  Matrix<2, 2> mat1{arr1};
  Matrix<2, 2> mat2{arr2};

  Matrix<2, 2> mat3{};
  mat1.Add(mat2, mat3);

  REQUIRE(mat3.Get(0, 0) == 6);
  REQUIRE(mat3.Get(0, 1) == 8);
  REQUIRE(mat3.Get(1, 0) == 10);
  REQUIRE(mat3.Get(1, 1) == 12);
}