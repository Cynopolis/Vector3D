#pragma once

#include <cstdint>
#include <cmath>

template <typename Type>
class V3D{
    public:
    constexpr V3D(const V3D& other):
    x(other.x),
    y(other.y),
    z(other.z){
        static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
    }
    
    constexpr V3D(Type x=0, Type y=0, Type z=0): 
    x(x), 
    y(y), 
    z(z){
        static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
    }

    template <typename OtherType>
    constexpr V3D(const V3D<OtherType> other):
    x(static_cast<Type>(other.x)),
    y(static_cast<Type>(other.y)),
    z(static_cast<Type>(other.z)){
        static_assert(std::is_arithmetic<Type>::value, "Type must be a number");
        static_assert(std::is_arithmetic<OtherType>::value, "OtherType must be a number");
    }

    V3D& operator=(const V3D &other){
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    V3D& operator+=(const V3D &other){
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    V3D& operator-=(const V3D &other){
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    V3D& operator/=(const Type scalar){
        if(scalar == 0){
            return *this;
        }
        this->x /= scalar;
        this->y /= scalar;
        this->z /= scalar;
        return *this;
    }

    V3D& operator*=(const Type scalar){
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        return *this;
    }

    bool operator==(const V3D &other){
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    float magnitude(){
        return std::sqrt(static_cast<float>(this->x * this->x + this->y * this->y + this->z * this->z));
    }
    Type x;
    Type y;
    Type z;
};