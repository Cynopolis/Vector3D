#pragma once

#include <array>
#include <cstdint>
#include <string>

// TODO: Add a function to calculate eigenvalues/vectors
// TODO: Add a function to compute RREF
// TODO: Add a function for SVD decomposition
// TODO: Add a function for LQ decomposition

template <uint8_t rows, uint8_t columns> class Matrix {
public:
  static_assert(rows > 0, "Template error: rows must be greater than 0.");
  static_assert(columns > 0, "Template error: columns must be greater than 0.");
  /**
   * @brief create a matrix but leave all of its values unitialized
   */
  Matrix() = default;

  /**
   * @brief Initialize a matrix with an array
   */
  Matrix(const std::array<float, rows * columns> &array);

  /**
   * @brief Initialize a matrix as a copy of another matrix
   */
  Matrix(const Matrix<rows, columns> &other);

  /**
   * @brief Initialize a matrix directly with any number of arguments
   */
  template <typename... Args> Matrix(Args... args);

  /**
   * @brief Create an identity matrix
   */
  static Matrix<rows, columns> Identity();

  /**
   * @brief Set all elements in this to value
   */
  void Fill(float value);

  /**
   * @brief Element-wise matrix addition
   * @param other the other matrix to add to this one
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  Matrix<rows, columns> &Add(const Matrix<rows, columns> &other,
                             Matrix<rows, columns> &result) const;

  /**
   * @brief Element-wise subtract matrix
   * @param other the other matrix to subtract from this one
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  Matrix<rows, columns> &Sub(const Matrix<rows, columns> &other,
                             Matrix<rows, columns> &result) const;

  /**
   * @brief Matrix multiply the two matrices
   * @param other the other matrix to multiply into this one
   * @param result A buffer to store the result into
   */
  template <uint8_t other_columns>
  Matrix<rows, other_columns> &Mult(const Matrix<columns, other_columns> &other,
                                    Matrix<rows, other_columns> &result) const;

  /**
   * @brief Multiply the matrix by a scalar
   * @param scalar the the scalar to multiply by
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  Matrix<rows, columns> &Mult(float scalar,
                              Matrix<rows, columns> &result) const;

  /**
   * @brief Element-wise multiply the two matrices
   * @param other the other matrix to multiply into this one
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  Matrix<rows, columns> &ElementMultiply(const Matrix<rows, columns> &other,
                                         Matrix<rows, columns> &result) const;

  /**
   * @brief Element-wise divide the two matrices
   * @param other the other matrix to multiply into this one
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  Matrix<rows, columns> &ElementDivide(const Matrix<rows, columns> &other,
                                       Matrix<rows, columns> &result) const;

  Matrix<rows - 1, columns - 1> &
  MinorMatrix(Matrix<rows - 1, columns - 1> &result, uint8_t row_idx,
              uint8_t column_idx) const;

  /**
   * @return Get the determinant of the matrix
   * @note for right now only 2x2 and 3x3 matrices are supported
   */
  float Det() const;

  Matrix<rows, columns> &MatrixOfMinors(Matrix<rows, columns> &result) const;

  /**
   * @brief Invert this matrix
   * @param result A buffer to store the result into
   * @warning this is super slow! Only call it if you absolutely have to!!!
   */
  Matrix<rows, columns> Invert() const;

  /**
   * @brief Transpose this matrix
   * @param result A buffer to store the result into
   */
  Matrix<columns, rows> Transpose() const;

  /**
   * @brief Returns the euclidean magnitude of the matrix. Also known as the L2
   * norm
   * @param result a buffer to store the result into
   */
  float EuclideanNorm() const;

  /**
   * @brief Get a row from the matrix
   * @param row_index the row index to get
   * @param row a buffer to write the row into
   */
  Matrix<1, columns> &GetRow(uint8_t row_index, Matrix<1, columns> &row) const;

  /**
   * @brief Get a row from the matrix
   * @param column_index the row index to get
   * @param column a buffer to write the row into
   */
  Matrix<rows, 1> &GetColumn(uint8_t column_index,
                             Matrix<rows, 1> &column) const;

  /**
   * @brief Get the number of rows in this matrix
   */
  constexpr uint8_t GetRowSize() { return rows; }

  /**
   * @brief Get the number of columns in this matrix
   */
  constexpr uint8_t GetColumnSize() { return columns; }

  /**
   * @brief Write a string representation of the matrix into the buffer
   */
  void ToString(std::string &stringBuffer) const;

  /**
   * @brief Returns the internal representation of the matrix as an array
   */
  const float *ToArray() const;

  /**
   * @brief Get an element from the matrix
   * @param row the row index of the element
   * @param column the column index of the element
   * @return The value of the element you want to get
   */
  float Get(uint8_t row_index, uint8_t column_index) const;

  /**
   * @brief get the specified row of the matrix returned as a reference to the
   * internal array
   */
  std::array<float, columns> &operator[](uint8_t row_index);

  /**
   * @brief Copy the contents of other into this matrix
   */
  Matrix<rows, columns> &operator=(const Matrix<rows, columns> &other);

  /**
   * @brief Return a new matrix that is the sum of this matrix and other matrix
   */
  Matrix<rows, columns> operator+(const Matrix<rows, columns> &other) const;

  Matrix<rows, columns> operator-(const Matrix<rows, columns> &other) const;

  template <uint8_t other_columns>
  Matrix<rows, other_columns>
  operator*(const Matrix<columns, other_columns> &other) const;

  Matrix<rows, columns> operator*(float scalar) const;

  Matrix<rows, columns> operator/(float scalar) const;

  template <uint8_t sub_rows, uint8_t sub_columns, uint8_t row_offset,
            uint8_t column_offset>
  Matrix<sub_rows, sub_columns> SubMatrix() const;

  template <uint8_t sub_rows, uint8_t sub_columns>
  void SetSubMatrix(uint8_t rowOffset, uint8_t columnOffset,
                    const Matrix<sub_rows, sub_columns> &sub_matrix);

  /**
   * @brief take the dot product of the two vectors
   */
  template <uint8_t vector_size>
  static float DotProduct(const Matrix<1, vector_size> &vec1,
                          const Matrix<1, vector_size> &vec2);

  template <uint8_t vector_size>
  static float DotProduct(const Matrix<vector_size, 1> &vec1,
                          const Matrix<vector_size, 1> &vec2);

  static float DotProduct(const Matrix<1, 1> &vec1, const Matrix<1, 1> &vec2) {
    return vec1.Get(0, 0) * vec2.Get(0, 0);
  }

  /**
   * @brief Performs QR decomposition on this matrix
   * @param Q a buffer that will contain Q after the function completes
   * @param R a buffer that will contain R after the function completes
   */
  void QRDecomposition(Matrix<rows, columns> &Q,
                       Matrix<columns, columns> &R) const;

  /**
   * @brief Uses QR decomposition to efficiently calculate the eigenvectors
   * and values of this matrix
   * @param eigenVectors a buffer that will contain the eigenvectors fo this
   * matrix
   * @param eigenValues a buffer that will contain the eigenValues fo this
   * matrix
   * @param maxIterations the number of iterations to perform before giving
   * up on reaching the given tolerance
   * @param tolerance the level of accuracy to obtain before stopping.
   */
  void EigenQR(Matrix<rows, rows> &eigenVectors, Matrix<rows, 1> &eigenValues,
               uint32_t maxIterations = 1000, float tolerance = 1e-6f) const;

protected:
  std::array<float, rows * columns> matrix;

private:
  Matrix<rows, columns> &adjugate(Matrix<rows, columns> &result) const;

  void setMatrixToArray(const std::array<float, rows * columns> &array);
};

#ifndef MATRIX_H_
#include "Matrix.cpp"
#endif // MATRIX_H_