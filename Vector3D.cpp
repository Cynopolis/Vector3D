#ifdef MATRIX_H_ // since the .cpp file has to be included by the .hpp file this
                 // will evaluate to true
#include "Vector3D.hpp"

template <typename Type>
constexpr V3D<type>::V3D(const Matrix<1, 3> &other) : x(other[0][0]), y(other[0][1]), z(other[0][2]) {}

template <typename Type>
constexpr V3D<type>::V3D(const Matrix<3, 1> &other) : x(other[0][0]), y(other[1][0]), z(other[2][0]) {}

template <typename Type>
constexpr V3D<type>::V3D(const V3D &other) : x(other.x),
                                             y(other.y),
                                             z(other.z)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
}

template <typename Type>
constexpr V3D<type>::V3D(Type x = 0, Type y = 0, Type z = 0) : x(x),
                                                               y(y),
                                                               z(z)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
}

template <typename Type, typename OtherType>
constexpr V3D<type>::V3D(const V3D<OtherType> other) : x(static_cast<Type>(other.x)),
                                                       y(static_cast<Type>(other.y)),
                                                       z(static_cast<Type>(other.z))
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
    static_assert(std::is_arithmetic<OtherType>::value, "OtherType must be a number");
}

template <typename Type>
std::array<Type, 3> V3D<type>::ToArray()
{
    return {this->x, this->y, this->z};
}

template <typename Type>
V3D &V3D<type>::operator=(const V3D &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

template <typename Type>
V3D &V3D<type>::operator+=(const V3D &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

template <typename Type>
V3D &V3D<type>::operator-=(const V3D &other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

template <typename Type>
V3D &V3D<type>::operator/=(const Type scalar)
{
    if (scalar == 0)
    {
        return *this;
    }
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    return *this;
}

template <typename Type>
V3D &V3D<type>::operator*=(const Type scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
}

template <typename Type>
bool V3D<type>::operator==(const V3D &other)
{
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

template <typename Type>
float V3D<type>::magnitude()
{
    return std::sqrt(static_cast<float>(this->x * this->x + this->y * this->y + this->z * this->z));
}

#endif // MATRIX_H_