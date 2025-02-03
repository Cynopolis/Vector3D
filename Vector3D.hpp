#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <cstdint>
#include <cmath>
#include <type_traits>

#include "Matrix.hpp"

template <typename Type>
class V3D
{
public:
    constexpr V3D(const Matrix<1, 3> &other);
    constexpr V3D(const Matrix<3, 1> &other);

    constexpr V3D(const V3D &other);

    constexpr V3D(Type x = 0, Type y = 0, Type z = 0);

    template <typename OtherType>
    constexpr V3D(const V3D<OtherType> other);

    std::array<Type, 3> ToArray();

    V3D &operator=(const V3D &other);

    V3D &operator+=(const V3D &other);

    V3D &operator-=(const V3D &other);

    V3D &operator/=(const Type scalar);

    V3D &operator*=(const Type scalar);

    bool operator==(const V3D &other);

    float magnitude();
    Type x;
    Type y;
    Type z;
};

#endif // VECTOR3D_H_