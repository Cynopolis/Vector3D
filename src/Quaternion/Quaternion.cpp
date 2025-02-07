#include "Quaternion.h"
#include <cmath>

float Quaternion::operator[](uint8_t index)
{
    if (index < 4)
    {
        return this->matrix[index];
    }

    // index out of bounds
    return 1e+6;
}

Quaternion Quaternion::operator+(const Quaternion &other)
{
    return Quaternion{this->v1 * other.v1, this->v2 * other.v2, this->v3 * other.v3, this->w * other.w};
}

Quaternion &
Quaternion::Rotate(Quaternion &other, Quaternion &buffer)
{
    // eq. 6
    buffer.v1 = this->w * other.v1 + other.w * this->v1 - this->v2 * other.v3 + this->v3 * other.v2;
    buffer.v2 = this->w * other.v2 + other.w * this->v2 - this->v3 * other.v1 + this->v1 * other.v3;
    buffer.v3 = this->w * other.v3 + other.w * this->v3 - this->v1 * other.v2 + this->v2 * other.v1;
    buffer.w = this->w * other.w - this->v1 * other.v1 - this->v2 * other.v2 - this->v3 * other.v3;
    return buffer;
}

Matrix<1, 3> &
Quaternion::ToEulerAngles(Matrix<1, 3> &angleBuffer)
{
    // from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
    // rotation sequence R = Rx * Ry * Rz
    // roll (x-axis rotation)
    float sinr_cosp = 2 * (this->v2 * this->v3 - this->w * this->v1);
    float cosr_cosp = 1 - 2 * (this->v1 * this->v1 + this->v2 * this->v2);
    angleBuffer[0][0] = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = -2 * (this->w * this->v2 + this->v3 * this->v1);
    if (abs(sinp) >= 1)
        angleBuffer[0][1] = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angleBuffer[0][1] = asin(sinp);

    // yaw (z-axis rotation)
    float siny_cosp = 2 * (this->v1 * this->v2 - this->w * this->v3);
    float cosy_cosp = 1 - 2 * (this->v2 * this->v2 + this->v3 * this->v3);
    angleBuffer[0][2] = atan2(siny_cosp, cosy_cosp);

    return angleBuffer;
}

Matrix<3, 3> &Quaternion::ToRotationMatrix(Matrix<3, 3> &rotationMatrixBuffer)
{
    // eq. 4
    // from https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
    Matrix<3, 3> temp{1.0 - 2.0 * this->v2 * this->v2 - 2.0 * this->v3 * this->v3, 2.0 * this->v1 * this->v2 - 2.0 * this->v3 * this->w, 2.0 * this->v1 * this->v3 + 2.0 * this->v2 * this->w,
                      2.0 * this->v1 * this->v2 + 2.0 * this->v3 * this->w, 1.0 - 2.0 * this->v1 * this->v1 - 2.0 * this->v3 * this->v3, 2.0 * this->v2 * this->v3 - 2.0 * this->v1 * this->w,
                      2.0 * this->v1 * this->v3 - 2.0 * this->v2 * this->w, 2.0 * this->v2 * this->v3 + 2.0 * this->v1 * this->w, 1.0 - 2.0 * this->v1 * this->v1 - 2.0 * this->v2 * this->v2};

    temp.Transpose(rotationMatrixBuffer);
    return rotationMatrixBuffer;
}