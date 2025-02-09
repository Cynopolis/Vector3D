#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Matrix.hpp"
class Quaternion : public Matrix<1, 4>
{
public:
    Quaternion() : Matrix<1, 4>() {}
    Quaternion(float fillValue) : Matrix<1, 4>(fillValue) {}
    Quaternion(float w, float v1, float v2, float v3) : Matrix<1, 4>(w, v1, v2, v3) {}
    Quaternion(const Quaternion &q) : Matrix<1, 4>(q.w, q.v1, q.v2, q.v3) {}
    Quaternion(const Matrix<1, 4> &matrix) : Matrix<1, 4>(matrix) {}
    Quaternion(const std::array<float, 4> &array) : Matrix<1, 4>(array) {}

    /**
     * @brief Create a quaternion from an angle and axis
     * @param angle The angle to rotate by
     * @param axis The axis to rotate around
     */
    static Quaternion FromAngleAndAxis(float angle, const Matrix<1, 3> &axis);

    /**
     * @brief Access the elements of the quaternion
     * @param index The index of the element to access
     * @return The value of the element at the index
     */
    float operator[](uint8_t index) const;

    /**
     * @brief Assign one quaternion to another
     */
    void operator=(const Quaternion &other) const;

    /**
     * @brief Do quaternion multiplication
     */
    Quaternion operator*(const Quaternion &other) const;

    /**
     * @brief Multiply the quaternion by a scalar
     */
    Quaternion operator*(float scalar) const;

    /**
     * @brief Add two quaternions together
     * @param other The quaternion to add to this one
     * @return The net quaternion
     */
    Quaternion operator+(const Quaternion &other) const;

    /**
     * @brief Q_Mult a quaternion by another quaternion
     * @param other The quaternion to rotate by
     * @param buffer The buffer to store the result in
     * @return A reference to the buffer
     */
    Quaternion &Q_Mult(const Quaternion &other, Quaternion &buffer) const;

    /**
     * @brief Rotate a quaternion by this quaternion
     * @param other The quaternion to rotate
     * @param buffer The buffer to store the result in
     *
     */
    Quaternion &Rotate(Quaternion &other, Quaternion &buffer) const;

    /**
     * @brief Normalize the quaternion to a magnitude of 1
     */
    void Normalize();

    Matrix<3, 3> ToRotationMatrix() const;

    // Give people an easy way to access the elements
    float &w{matrix[0]};
    float &v1{matrix[1]};
    float &v2{matrix[2]};
    float &v3{matrix[3]};
};

#endif // QUATERNION_H_