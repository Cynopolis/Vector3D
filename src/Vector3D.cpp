#ifdef VECTOR3D_H_ // since the .cpp file has to be included by the .hpp file this
                   // will evaluate to true
#include <cmath>
#include <type_traits>

template <typename Type>
V3D<Type>::V3D(const Matrix<1, 3> &other)
{
    this->x = other.Get(0, 0);
    this->y = other.Get(0, 1);
    this->z = other.Get(0, 2);
}

template <typename Type>
V3D<Type>::V3D(const Matrix<3, 1> &other)
{
    this->x = other.Get(0, 0);
    this->y = other.Get(1, 0);
    this->z = other.Get(2, 0);
}

template <typename Type>
V3D<Type>::V3D(const V3D &other) : x(other.x),
                                   y(other.y),
                                   z(other.z)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
}

template <typename Type>
V3D<Type>::V3D(Type x, Type y, Type z) : x(x),
                                         y(y),
                                         z(z)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
}

template <typename Type>
template <typename OtherType>
V3D<Type>::V3D(const V3D<OtherType> &other)
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
    static_assert(std::is_arithmetic<OtherType>::value, "OtherType must be a number");
    this->x = static_cast<Type>(other.x);
    this->y = static_cast<Type>(other.y);
    this->z = static_cast<Type>(other.z);
}

template <typename Type>
std::array<Type, 3> V3D<Type>::ToArray() const
{
    return {this->x, this->y, this->z};
}

template <typename Type>
void V3D<Type>::operator=(const V3D<Type> &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

template <typename Type>
V3D<Type> V3D<Type>::operator+(const V3D<Type> &other) const
{
    return V3D<Type>{this->x + other.x, this->y + other.y, this->z + other.z};
}

template <typename Type>
V3D<Type> V3D<Type>::operator-(const V3D<Type> &other) const
{
    return V3D<Type>{this->x - other.x, this->y - other.y, this->z - other.z};
}

template <typename Type>
V3D<Type> V3D<Type>::operator*(Type scalar) const
{
    return V3D<Type>{this->x * scalar, this->y * scalar, this->z * scalar};
}

template <typename Type>
V3D<Type> V3D<Type>::operator/(Type scalar) const
{
    return V3D<Type>{this->x / scalar, this->y / scalar, this->z / scalar};
}

template <typename Type>
V3D<Type> &V3D<Type>::operator+=(const V3D<Type> &other)
{
    *this = *this + other;
    return *this;
}

template <typename Type>
V3D<Type> &V3D<Type>::operator-=(const V3D<Type> &other)
{
    *this = *this - other;
    return *this;
}

template <typename Type>
V3D<Type> &V3D<Type>::operator/=(Type scalar)
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
V3D<Type> &V3D<Type>::operator*=(Type scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
}

template <typename Type>
bool V3D<Type>::operator==(const V3D<Type> &other)
{
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

template <typename Type>
float V3D<Type>::magnitude()
{
    return std::sqrt(static_cast<float>(this->x * this->x + this->y * this->y + this->z * this->z));
}

#endif // VECTOR3D_H_