#ifndef MATRIX_H_
#define MATRIX_H_

#include <array>
#include <cstdint>

// TODO: Add a function to calculate eigenvalues/vectors
// TODO: Add a function to compute RREF
// TODO: Add a function for SVD decomposition
// TODO: Add a function for LQ decomposition

template <uint8_t rows, uint8_t columns> class Matrix {
public:
  /**
   * @brief create a matrix but leave all of its values unitialized
   */
  Matrix() = default;

  /**
   * @brief Create a matrix but fill all of its entries with one value
   */
  Matrix(float value);

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
  Matrix<rows, columns> &Invert(Matrix<rows, columns> &result) const;

  /**
   * @brief Transpose this matrix
   * @param result A buffer to store the result into
   */
  Matrix<columns, rows> &Transpose(Matrix<columns, rows> &result) const;

  /**
   * @brief reduce the matrix so the sum of its elements equal 1
   * @param result a buffer to store the result into
   */
  Matrix<rows, columns> &Normalize(Matrix<rows, columns> &result) const;

  /**
   * @brief return an identity matrix of the specified size
   */
  static Matrix<rows, rows> Eye();

  /**
   * @brief write a copy of a sub matrix into the given result matrix.
   * @param rowIndex The row index to start the copy from
   * @param columnIndex the column index to start the copy from
   * @param result the matrix buffer to write the sub matrix into. The size of
   * the matrix buffer allows the function to determine the end indices of the
   * sub matrix
   */
  template <uint8_t subRows, uint8_t subColumns>
  Matrix<subRows, subColumns> &
  SubMatrix(uint8_t rowIndex, uint8_t columnIndex,
            Matrix<subRows, subColumns> &result) const {
    return result;
  }

  /**
   * @brief write a copy of a sub matrix into this matrix starting at the given
   * idnex.
   * @param rowIndex The row index to start the copy from
   * @param columnIndex the column index to start the copy from
   * @param subMatrix The submatrix to copy into this matrix. The size of
   * the matrix buffer allows the function to determine the end indices of the
   * sub matrix
   */
  template <uint8_t subRows, uint8_t subColumns>
  void CopySubMatrixInto(uint8_t rowIndex, uint8_t columnIndex,
                         const Matrix<subRows, subColumns> &subMatrix) {}

  /**
   * @brief Returns the norm of the matrix
   */
  float Norm() { return 0; }

  template <uint8_t vec1Length, uint8_t vec2Length>
  static Matrix<vec1Length, vec2Length> &
  OuterProduct(const Matrix<1, vec1Length> &vec1,
               const Matrix<1, vec2Length> &vec2,
               Matrix<vec1Length, vec2Length> &result) {
    return result;
  }

  template <uint8_t vec1Length, uint8_t vec2Length>
  static Matrix<vec1Length, vec2Length> &
  OuterProduct(const Matrix<vec1Length, 1> &vec1,
               const Matrix<vec2Length, 1> &vec2,
               Matrix<vec1Length, vec2Length> &result) {
    return result;
  }
  /**
   * @brief Calulcate the QR decomposition of a matrix
   * @param Q the
   */
  void QR_Decomposition(Matrix<rows, columns> &Q,
                        Matrix<rows, columns> &R) const;

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

  void ToString(std::string &stringBuffer) const;

  /**
   * @brief Get an element from the matrix
   * @param row the row index of the element
   * @param column the column index of the element
   * @return The value of the element you want to get
   */
  float Get(uint8_t row_index, uint8_t column_index) const;

  // *******************************************************
  // ************** OPERATOR OVERRIDES *********************
  // *******************************************************

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

  Matrix<rows, columns> operator*(const Matrix<rows, columns> &other) const;

  Matrix<rows, columns> operator*(float scalar) const;

private:
  /**
   * @brief take the dot product of the two vectors
   */
  template <uint8_t vector_size>
  static float dotProduct(const Matrix<1, vector_size> &vec1,
                          const Matrix<1, vector_size> &vec2);

  template <uint8_t vector_size>
  static float dotProduct(const Matrix<vector_size, 1> &vec1,
                          const Matrix<vector_size, 1> &vec2);

  Matrix<rows, columns> &adjugate(Matrix<rows, columns> &result) const;

  void setMatrixToArray(const std::array<float, rows * columns> &array);

  std::array<float, rows * columns> matrix;
};

#include "Matrix.cpp"

#endif // MATRIX_H_