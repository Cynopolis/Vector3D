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
        REQUIRE(q1.v1 == 1);
        REQUIRE(q1.v2 == 2);
        REQUIRE(q1.v3 == 3);
        REQUIRE(q1.w == 4);

        // fill initialization
        Quaternion q3{0};
        REQUIRE(q3.v1 == 0);
        REQUIRE(q3.v2 == 0);
        REQUIRE(q3.v3 == 0);
        REQUIRE(q3.w == 0);

        // copy initialization
        Quaternion q4{q1};
        REQUIRE(q4.v1 == 1);
        REQUIRE(q4.v2 == 2);
        REQUIRE(q4.v3 == 3);
        REQUIRE(q4.w == 4);

        // matrix initialization
        Matrix<1, 4> m1{1, 2, 3, 4};
        Quaternion q5{m1};
        REQUIRE(q5.v1 == 1);
        REQUIRE(q5.v2 == 2);
        REQUIRE(q5.v3 == 3);
        REQUIRE(q5.w == 4);

        // array initialization
        Quaternion q6{std::array<float, 4>{1, 2, 3, 4}};
        REQUIRE(q6.v1 == 1);
        REQUIRE(q6.v2 == 2);
        REQUIRE(q6.v3 == 3);
        REQUIRE(q6.w == 4);
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
        REQUIRE(q3.v1 == 5);
        REQUIRE(q3.v2 == 12);
        REQUIRE(q3.v3 == 21);
        REQUIRE(q3.w == 32);
    }

    SECTION("Rotation")
    {
        Quaternion q3;
        q1.Rotate(q2, q3);
        REQUIRE(q3.v1 == -24);
        REQUIRE(q3.v2 == 48);
        REQUIRE(q3.v3 == 48);
        REQUIRE(q3.w == -6);
    }

    SECTION("Euler Angles")
    {
        Matrix<1, 3> angles;
        q1.ToEulerAngles(angles);
        REQUIRE_THAT(angles.Get(0, 0), Catch::Matchers::WithinRel(-0.1973956f, 1e-6f));
        REQUIRE_THAT(angles.Get(0, 1), Catch::Matchers::WithinRel(0.823212f, 1e-6f));
        REQUIRE_THAT(angles.Get(0, 2), Catch::Matchers::WithinRel(1.3734008f, 1e-6f));
    }

    SECTION("Rotation Matrix")
    {
        Matrix<3, 3> rotationMatrix;
        q1.ToRotationMatrix(rotationMatrix);
        REQUIRE_THAT(rotationMatrix.Get(0, 0), Catch::Matchers::WithinRel(0.1333333f, 1e-6f));
        REQUIRE_THAT(rotationMatrix.Get(0, 1), Catch::Matchers::WithinRel(-0.6666667f, 1e-6f));
        REQUIRE_THAT(rotationMatrix.Get(0, 2), Catch::Matchers::WithinRel(0.7333333f, 1e-6f));
        REQUIRE_THAT(rotationMatrix.Get(1, 0), Catch::Matchers::WithinRel(0.9333333f, 1e-6f));
        REQUIRE_THAT(rotationMatrix.Get(1, 1), Catch::Matchers::WithinRel(0.3333333f, 1e-6f));
        REQUIRE_THAT(rotationMatrix.Get(1, 2), Catch::Matchers::WithinRel(0.1333333f, 1e-6f));
        REQUIRE_THAT(rotationMatrix.Get(2, 0), Catch::Matchers::WithinRel(-0.3333333f, 1e-6f));
        REQUIRE_THAT(rotationMatrix.Get(2, 1), Catch::Matchers::WithinRel(0.6666667f, 1e-6f));
        REQUIRE_THAT(rotationMatrix.Get(2, 2), Catch::Matchers::WithinRel(0.6666667f, 1e-6f));
    }
}