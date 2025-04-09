#include "Quaternion.h"
#include <cmath>

/**
 * @brief Create a quaternion from an angle and axis
 * @param angle The angle to rotate by
 * @param axis The axis to rotate around
 */
Quaternion Quaternion::FromAngleAndAxis(float angle, const Matrix<1, 3> &axis)
{
    const float halfAngle = angle / 2;
    const float sinHalfAngle = sin(halfAngle);
    Matrix<1, 3> normalizedAxis{};
    axis.Normalize(normalizedAxis);
    return Quaternion{
        static_cast<float>(cos(halfAngle)),
        normalizedAxis.Get(0, 0) * sinHalfAngle,
        normalizedAxis.Get(0, 1) * sinHalfAngle,
        normalizedAxis.Get(0, 2) * sinHalfAngle};
}

float Quaternion::operator[](uint8_t index) const
{
    if (index < 4)
    {
        return this->matrix[index];
    }

    // index out of bounds
    return 1e+6;
}

void Quaternion::operator=(const Quaternion &other)
{
    memcpy(&(this->matrix), &(other.matrix), 4 * sizeof(float));
}

Quaternion Quaternion::operator*(const Quaternion &other) const
{
    Quaternion result{};
    this->Q_Mult(other, result);
    return result;
}

Quaternion Quaternion::operator*(float scalar) const
{
    return Quaternion{this->w * scalar, this->v1 * scalar, this->v2 * scalar, this->v3 * scalar};
}

Quaternion Quaternion::operator+(const Quaternion &other) const
{
    return Quaternion{this->w + other.w, this->v1 + other.v1, this->v2 + other.v2, this->v3 + other.v3};
}

Quaternion &
Quaternion::Q_Mult(const Quaternion &other, Quaternion &buffer) const
{

    // eq. 6
    buffer.w = (other.w * this->w - other.v1 * this->v1 - other.v2 * this->v2 - other.v3 * this->v3);
    buffer.v1 = (other.w * this->v1 + other.v1 * this->w - other.v2 * this->v3 + other.v3 * this->v2);
    buffer.v2 = (other.w * this->v2 + other.v1 * this->v3 + other.v2 * this->w - other.v3 * this->v1);
    buffer.v3 = (other.w * this->v3 - other.v1 * this->v2 + other.v2 * this->v1 + other.v3 * this->w);
    return buffer;
}

Quaternion &Quaternion::Rotate(Quaternion &other, Quaternion &buffer) const
{
    Quaternion prime{this->w, -this->v1, -this->v2, -this->v3};
    buffer.v1 = other.v1;
    buffer.v2 = other.v2;
    buffer.v3 = other.v3;
    buffer.w = 0;

    Quaternion temp{};
    this->Q_Mult(buffer, temp);
    temp.Q_Mult(prime, buffer);
    return buffer;
}

void Quaternion::Normalize()
{
    float magnitude = sqrt(this->v1 * this->v1 + this->v2 * this->v2 + this->v3 * this->v3 + this->w * this->w);
    if (magnitude == 0)
    {
        return;
    }
    this->v1 /= magnitude;
    this->v2 /= magnitude;
    this->v3 /= magnitude;
    this->w /= magnitude;
}

Matrix<3, 3> Quaternion::ToRotationMatrix() const
{
    float xx = this->v1 * this->v1;
    float yy = this->v2 * this->v2;
    float zz = this->v3 * this->v3;
    Matrix<3, 3> rotationMatrix{
        1 - 2 * (yy - zz), 2 * (this->v1 * this->v2 - this->v3 * this->w), 2 * (this->v1 * this->v3 + this->v2 * this->w),
        2 * (this->v1 * this->v2 + this->v3 * this->w), 1 - 2 * (xx - zz), 2 * (this->v2 * this->v3 - this->v1 * this->w),
        2 * (this->v1 * this->v3 - this->v2 * this->w), 2 * (this->v2 * this->v3 + this->v1 * this->w), 1 - 2 * (xx - yy)};
    return rotationMatrix;
};

Matrix<3, 1> Quaternion::ToEulerAngle() const
{
    float sqv1 = this->v1 * this->v1;
    float sqv2 = this->v2 * this->v2;
    float sqv3 = this->v3 * this->v3;
    float sqw = this->w * this->w;

    Matrix<3, 1> eulerAngle;
    {
        atan2(2.0 * (this->v1 * this->v2 + this->v3 * this->w), (sqv1 - sqv2 - sqv3 + sqw));
        asin(-2.0 * (this->v1 * this->v3 - this->v2 * this->w) / (sqv1 + sqv2 + sqv3 + sqw));
        atan2(2.0 * (this->v2 * this->v3 + this->v1 * this->w), (-sqv1 - sqv2 + sqv3 + sqw));
    };
    return eulerAngle;
}