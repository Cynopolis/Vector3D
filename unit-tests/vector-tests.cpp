// include the unit test framework first
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// include the module you're going to test next
#include "Vector3D.hpp"
#include "Matrix.hpp"

// any other libraries
#include <array>
#include <cmath>
#include <iostream>

TEST_CASE("Vector Math", "Vector")
{
    V3D<float> v1{1, 2, 3};
    V3D<float> v2{4, 5, 6};
    V3D<float> v3{};

    SECTION("Initialization")
    {
        // list initialization
        REQUIRE(v1.x == 1);
        REQUIRE(v1.y == 2);
        REQUIRE(v1.z == 3);

        // copy initialization
        V3D<float> v4{v2};
        REQUIRE(v4.x == 4);
        REQUIRE(v4.y == 5);
        REQUIRE(v4.z == 6);

        // empty initialization
        REQUIRE(v3.x == 0);
        REQUIRE(v3.y == 0);
        REQUIRE(v3.z == 0);

        // matrix initialization
        Matrix<1, 3> mat1{v1.ToArray()};
        V3D<float> v5{mat1};
        REQUIRE(v5.x == v1.x);
        REQUIRE(v5.y == v1.y);
        REQUIRE(v5.z == v1.z);
    }
}