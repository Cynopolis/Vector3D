#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

template <uint8_t rows, uint8_t columns> class Matrix {
public:
  Matrix();

  /**
   * @brief Initialize a matrix with an array
   */
  Matrix(const std::array<float, rows * columns> &array);

  Matrix(const Matrix<rows, columns> &other);
  // TODO: Figure out how to do this
  /**
   * @brief Initialize a matrix directly with any number of arguments
   */
  // template <typename... Args>
  // Matrix(Args&&... args);

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
  Matrix<rows, columns> &Mult(const Matrix<columns, other_columns> &other,
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
   * @brief Square this matrix
   * @param result A buffer to store the result into
   */
  Matrix<rows, columns> &Square(Matrix<rows, rows> &result) const;

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
  /**
   * @return Get the determinant of the matrix
   * @note for right now only 2x2 and 3x3 matrices are supported
   */
  float Det() const;

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

  /**
   * @brief get the specified row of the matrix returned as a reference to the
   * internal array
   */
  std::array<float, columns> &operator[](uint8_t row_index) {
    if (row_index > rows - 1) {
      return this->matrix[0]; // TODO: We should throw something here instead of
                              // failing quietly.
    }
    return this->matrix[row_index];
  }

  Matrix<rows, columns> &operator=(const Matrix<rows, columns> &other) {
    for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
      for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
        this->matrix[row_idx][column_idx] = other.Get(row_idx, column_idx);
      }
    }
    // return a reference to ourselves so you can chain together these functions
    return *this;
  }

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
  /**
   * @brief Set all elements in this matrix to zero
   */
  void zeroMatrix();

  Matrix<rows, columns> &matrixOfMinors(Matrix<rows, columns> &result) const;

  Matrix<rows - 1, columns - 1> &
      minorMatrix(Matrix<rows - 1, columns - 1> &result, uint8_t row_idx,
                  uint8_t column_idx) const;

  Matrix<rows, columns> &adjugate(Matrix<rows, columns> &result) const;

  void setMatrixToArray(const std::array<float, rows * columns> &array);

  std::array<std::array<float, columns>, rows> matrix;
};

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::setMatrixToArray(
    const std::array<float, rows * columns> &array) {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      uint16_t array_idx =
          static_cast<uint16_t>(row_idx) * static_cast<uint16_t>(columns) +
          static_cast<uint16_t>(column_idx);
      if (array_idx < array.size()) {
        this->matrix[row_idx][column_idx] = array[array_idx];
      } else {
        this->matrix[row_idx][column_idx] = 0;
      }
    }
  }
}

template <uint8_t rows, uint8_t columns> Matrix<rows, columns>::Matrix() {
  this->zeroMatrix();
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns>::Matrix(const std::array<float, rows * columns> &array) {
  this->setMatrixToArray(array);
}

// template <uint8_t rows, uint8_t columns>
// template <typename... Args>
// Matrix<rows, columns>::Matrix(Args&&... args){

//   // Initialize a std::array with the arguments
//   if(typeid(args) == typeid(std::array<float, 4>)){
//     this->setMatrixToArray(args);
//   }
//   else{
//     std::array<float, rows*columns> values = {static_cast<float>(args)...};

//     // now store the array in our internal matrix
//     this->setMatrixToArray(values);
//   }

// }

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns>::Matrix(const Matrix<rows, columns> &other) {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      this->matrix[row_idx][column_idx] = other.Get(row_idx, column_idx);
    }
  }
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Add(const Matrix<rows, columns> &other,
                           Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) + other.Get(row_idx, column_idx);
    }
  }
  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Sub(const Matrix<rows, columns> &other,
                           Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) - other.Get(row_idx, column_idx);
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
template <uint8_t other_columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Mult(const Matrix<columns, other_columns> &other,
                            Matrix<rows, other_columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      // get our row
      Matrix<1, columns> this_row;
      this->GetRow(row_idx, this_row);
      // get the other matrices column
      Matrix<rows, 1> other_column;
      other.GetColumn(column_idx, other_column);
      // transpose the other matrix's column
      Matrix<1, rows> other_column_t;
      other_column.Transpose(other_column_t);

      // the result's index is equal to the dot product of these two vectors
      result[row_idx][column_idx] =
          Matrix<rows, columns>::dotProduct(this_row, other_column_t);
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Mult(float scalar, Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] = this->Get(row_idx, column_idx) * scalar;
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Invert(Matrix<rows, columns> &result) const {
  // since all matrix sizes have to be statically specified at compile time we
  // can do this
  static_assert(rows == columns,
                "Your matrix isn't square and can't be inverted");

  // unfortunately we can't calculate this at compile time so we'll just reurn
  // zeros
  float determinant{this->Det()};
  if (this->Det() < 0) {
    // you can't invert a matrix with a negative determinant
    result.zeroMatrix();
    return result;
  }

  // TODO: This algorithm is really inneficient because of the matrix of minors.
  // We should make a different algorithm how to calculate the inverse:
  // https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html

  // calculate the matrix of minors
  Matrix<rows, columns> minors{};
  this->matrixOfMinors(minors);

  // now adjugate the matrix and save it in our output
  minors.adjugate(result);

  // scale the result by 1/determinant and we have our answer
  result.Mult(1 / determinant);

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<columns, rows> &
Matrix<rows, columns>::Transpose(Matrix<columns, rows> &result) const {
  for (uint8_t column_idx{0}; column_idx < rows; column_idx++) {
    for (uint8_t row_idx{0}; row_idx < columns; row_idx++) {
      result[row_idx][column_idx] = this->Get(column_idx, row_idx);
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Square(Matrix<rows, rows> &result) const {
  // TODO: Because template requirements are checked before static_assert, this
  // never throws an error and fails at the Mult call instead.
  static_assert(rows == columns, "You can't square a non-square matrix.");

  this->Mult(*this, result);

  return result;
}

// explicitly define the determinant for a 3x3 matrix because it is definitely
// the fastest way to calculte a 2x2 matrix determinant
template <> float Matrix<2, 2>::Det() const {
  return this->matrix[0][0] * this->matrix[1][1] -
         this->matrix[0][1] * this->matrix[1][1];
}

// explicitly define the determinant for a 3x3 matrix because it will probably
// be faster than the jacobi method for nxn matrices
template <> float Matrix<3, 3>::Det() const {
  float a{this->matrix[0][0]};
  float b{this->matrix[0][1]};
  float c{this->matrix[0][2]};

  Matrix<2, 2> minors{};
  this->minorMatrix(minors, 0, 0);
  float det = a * minors.Det();

  this->minorMatrix(minors, 0, 1);
  det -= b * minors.Det();

  this->minorMatrix(minors, 0, 2);
  det += c * minors.Det();

  return det;
}

template <uint8_t rows, uint8_t columns>
float Matrix<rows, columns>::Det() const {
  static_assert(rows == columns,
                "You can't take the determinant of a non-square matrix.");
  // static_assert(
  //     false,
  //     "Right now this operation isn't supported for matrices bigger than
  //     3x3");
  // Matrix<1, columns> eigenValues{};
  // this->EigenValues(eigenValues);

  // float determinant{1};
  // for (uint8_t i{0}; i < columns; i++) {
  //   determinant *= eigenValues.Get(0, i);
  // }

  // return determinant;
  return 0;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::ElementMultiply(const Matrix<rows, columns> &other,
                                       Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) * other.Get(row_idx, column_idx);
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::ElementDivide(const Matrix<rows, columns> &other,
                                     Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) / other.Get(row_idx, column_idx);
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
float Matrix<rows, columns>::Get(uint8_t row_index,
                                 uint8_t column_index) const {
  if (row_index > rows - 1 || column_index > columns - 1) {
    return 0; // TODO: We should throw something here instead of failing quietly
  }
  return this->matrix[row_index][column_index];
}

template <uint8_t rows, uint8_t columns>
Matrix<1, columns> &
Matrix<rows, columns>::GetRow(uint8_t row_index,
                              Matrix<1, columns> &row) const {
  row = Matrix<1, columns>(this->matrix[row_index]);

  return row;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, 1> &
Matrix<rows, columns>::GetColumn(uint8_t column_index,
                                 Matrix<rows, 1> &column) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    column[row_idx][0] = this->Get(row_idx, column_index);
  }

  return column;
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::ToString(std::string &stringBuffer) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    stringBuffer += "|";
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      stringBuffer += std::to_string(this->matrix[row_idx][column_idx]);
      if (column_idx != columns - 1) {
        stringBuffer += "\t";
      }
    }
    stringBuffer += "|\n";
  }
}

template <uint8_t rows, uint8_t columns>
template <uint8_t vector_size>
float Matrix<rows, columns>::dotProduct(const Matrix<1, vector_size> &vec1,
                                        const Matrix<1, vector_size> &vec2) {
  float sum{0};
  for (uint8_t i{0}; i < vector_size; i++) {
    sum += vec1.Get(0, i) * vec2.Get(0, i);
  }

  return sum;
}

template <uint8_t rows, uint8_t columns>
template <uint8_t vector_size>
float Matrix<rows, columns>::dotProduct(const Matrix<vector_size, 1> &vec1,
                                        const Matrix<vector_size, 1> &vec2) {
  float sum{0};
  for (uint8_t i{0}; i < vector_size; i++) {
    sum += vec1.Get(i, 0) * vec2.Get(i, 0);
  }

  return sum;
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::zeroMatrix() {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      this->matrix[row_idx][column_idx] = 0;
    }
  }
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::matrixOfMinors(Matrix<rows, columns> &result) const {
  Matrix<rows - 1, columns - 1> minorMatrix{};

  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      this->minorMatrix(minorMatrix, row_idx, column_idx);
      result[row_idx][column_idx] = minorMatrix.Det();
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows - 1, columns - 1> &
Matrix<rows, columns>::minorMatrix(Matrix<rows - 1, columns - 1> &result,
                                   uint8_t row_idx, uint8_t column_idx) const {
  std::array<float, (rows - 1) * (columns - 1)> subArray{};

  for (uint8_t row_iter{0}; row_iter < rows; row_iter++) {
    for (uint8_t column_iter{0}; column_iter < columns; column_iter++) {
      uint16_t array_idx =
          static_cast<uint16_t>(row_iter) + static_cast<uint16_t>(column_iter);
      if (row_iter == row_idx || column_iter == column_idx) {
        continue;
      }
      subArray[array_idx] = this->Get(row_iter, column_iter);
    }
  }

  result = Matrix<rows - 1, columns - 1>{subArray};
  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::adjugate(Matrix<rows, columns> &result) const {
  for (uint8_t row_iter{0}; row_iter < rows; row_iter++) {
    for (uint8_t column_iter{0}; column_iter < columns; column_iter++) {
      float sign = ((row_iter + 1) % 2) == 0 ? -1 : 1;
      sign *= ((column_iter + 1) % 2) == 0 ? -1 : 1;
      result[row_iter][column_iter] = this->Get(row_iter, column_iter) * sign;
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Normalize(Matrix<rows, columns> &result) const {
  float sum{0};
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      float val{this->Get(row_idx, column_idx)};
      sum += val * val;
    }
  }

  if (sum == 0) {
    // this wouldn't do anything anyways
    result.zeroMatrix();
    return result;
  }

  sum = sqrt(sum);

  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] = this->Get(row_idx, column_idx) / sum;
    }
  }

  return result;
}