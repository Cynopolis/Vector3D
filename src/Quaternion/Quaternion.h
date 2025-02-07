#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Matrix.hpp"

class Quaternion : public Matrix<1, 4>
{
public:
    Quaternion() : Matrix<1, 4>() {}
    Quaternion(float fillValue) : Matrix<1, 4>(fillValue) {}
    Quaternion(float v1, float v2, float v3, float w) : Matrix<1, 4>(v1, v2, v3, w) {}
    Quaternion(const Quaternion &q) : Matrix<1, 4>(q.v1, q.v2, q.v3, q.w) {}
    Quaternion(const Matrix<1, 4> &matrix) : Matrix<1, 4>(matrix) {}
    Quaternion(const std::array<float, 4> &array) : Matrix<1, 4>(array) {}

    float operator[](uint8_t index);
    Quaternion operator+(const Quaternion &other);
    Quaternion &Rotate(Quaternion &other, Quaternion &buffer);
    Matrix<1, 3> &ToEulerAngles(Matrix<1, 3> &angleBuffer);
    Matrix<3, 3> &ToRotationMatrix(Matrix<3, 3> &rotationMatrixBuffer);

    // Give people an easy way to access the elements
    float &v1{matrix[0]};
    float &v2{matrix[1]};
    float &v3{matrix[2]};
    float &w{matrix[3]};

private:
};

#endif // QUATERNION_H_