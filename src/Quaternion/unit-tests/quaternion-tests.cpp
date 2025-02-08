// include the unit test framework first
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// include the module you're going to test next
#include "Quaternion.h"

// any other libraries
#include <array>
#include <cmath>
#include <iostream>

TEST_CASE("Vector Math", "Vector")
{
    Quaternion q1{1, 2, 3, 4};
    Quaternion q2{5, 6, 7, 8};

    SECTION("Initialization")
    {
        // explicit initialization
        REQUIRE(q1.w == 1);
        REQUIRE(q1.v1 == 2);
        REQUIRE(q1.v2 == 3);
        REQUIRE(q1.v3 == 4);

        // fill initialization
        Quaternion q3{0};
        REQUIRE(q3.w == 0);
        REQUIRE(q3.v1 == 0);
        REQUIRE(q3.v2 == 0);
        REQUIRE(q3.v3 == 0);

        // copy initialization
        Quaternion q4{q1};
        REQUIRE(q4.w == 1);
        REQUIRE(q4.v1 == 2);
        REQUIRE(q4.v2 == 3);
        REQUIRE(q4.v3 == 4);

        // matrix initialization
        Matrix<1, 4> m1{1, 2, 3, 4};
        Quaternion q5{m1};
        REQUIRE(q5.w == 1);
        REQUIRE(q5.v1 == 2);
        REQUIRE(q5.v2 == 3);
        REQUIRE(q5.v3 == 4);

        // array initialization
        Quaternion q6{std::array<float, 4>{1, 2, 3, 4}};
        REQUIRE(q6.w == 1);
        REQUIRE(q6.v1 == 2);
        REQUIRE(q6.v2 == 3);
        REQUIRE(q6.v3 == 4);
    }

    SECTION("Array access")
    {
        REQUIRE(q1[0] == 1);
        REQUIRE(q1[1] == 2);
        REQUIRE(q1[2] == 3);
        REQUIRE(q1[3] == 4);
    }

    SECTION("Addition")
    {
        Quaternion q3 = q1 + q2;
        REQUIRE(q3.w == 5);
        REQUIRE(q3.v1 == 12);
        REQUIRE(q3.v2 == 21);
        REQUIRE(q3.v3 == 32);
    }

    SECTION("Multiplication")
    {
        Quaternion q3;
        q1.Q_Mult(q2, q3);
        REQUIRE(q3.w == -60);
        REQUIRE(q3.v1 == 12);
        REQUIRE(q3.v2 == 30);
        REQUIRE(q3.v3 == 24);
    }

    SECTION("Rotation")
    {
        Quaternion q3{Quaternion::FromAngleAndAxis(M_PI / 2, Matrix<1, 3>{0, 0, 1})};
        Quaternion q4{0, 1, 0, 0};
        Quaternion q5;
        q3.Rotate(q4, q5);
        REQUIRE_THAT(q5.v1, Catch::Matchers::WithinRel(0.0f, 1e-6f));
        REQUIRE_THAT(q5.v2, Catch::Matchers::WithinRel(1.0f, 1e-6f));
        REQUIRE_THAT(q5.v3, Catch::Matchers::WithinRel(0.0f, 1e-6f));
    }
}