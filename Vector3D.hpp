#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <cstdint>
#include "Matrix.hpp"

template <typename Type>
class V3D
{
public:
    V3D(const Matrix<1, 3> &other);
    V3D(const Matrix<3, 1> &other);

    V3D(const V3D &other);

    V3D(Type x = 0, Type y = 0, Type z = 0);

    template <typename OtherType>
    V3D(const V3D<OtherType> &other);

    std::array<Type, 3> ToArray();

    V3D<Type> operator+(const V3D<Type> &other);

    V3D<Type> operator-(const V3D<Type> &other);

    V3D<Type> operator*(Type scalar);

    void operator=(const V3D<Type> &other);

    V3D<Type> &operator+=(const V3D<Type> &other);

    V3D<Type> &operator-=(const V3D<Type> &other);

    V3D<Type> &operator/=(Type scalar);

    V3D<Type> &operator*=(Type scalar);

    bool operator==(const V3D<Type> &other);

    float magnitude();

    Type x;
    Type y;
    Type z;
};

#include "Vector3D.cpp"
#endif // VECTOR3D_H_