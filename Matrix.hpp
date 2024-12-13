#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

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
  // TODO: Figure out how to do this
  /**
   * @brief Initialize a matrix directly with any number of arguments
   */
  // template <typename... Args>
  // Matrix(Args&&... args);
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

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns>::Matrix(float value) {
  this->Fill(value);
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
  // allocate some buffers for all of our dot products
  Matrix<1, columns> this_row;
  Matrix<rows, 1> other_column;
  Matrix<1, rows> other_column_t;

  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    // get our row
    this->GetRow(row_idx, this_row);
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      // get the other matrix'ss column
      other.GetColumn(column_idx, other_column);
      // transpose the other matrix's column
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
  if (determinant == 0) {
    // you can't invert a matrix with a negative determinant
    result.Fill(0);
    return result;
  }

  // TODO: This algorithm is really inneficient because of the matrix of minors.
  // We should make a different algorithm how to calculate the inverse:
  // https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html

  // calculate the matrix of minors
  Matrix<rows, columns> minors{};
  this->MatrixOfMinors(minors);

  // now adjugate the matrix and save it in our output
  minors.adjugate(result);

  // scale the result by 1/determinant and we have our answer
  result = result * (1 / determinant);
  // result.Mult(1 / determinant, result);

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

// explicitly define the determinant for a 2x2 matrix because it is definitely
// the fastest way to calculate a 2x2 matrix determinant
template <> float Matrix<0, 0>::Det() const { return 1e+6; }
template <> float Matrix<1, 1>::Det() const { return this->matrix[0][0]; }
template <> float Matrix<2, 2>::Det() const {
  return this->matrix[0][0] * this->matrix[1][1] -
         this->matrix[0][1] * this->matrix[1][0];
}

template <uint8_t rows, uint8_t columns>
float Matrix<rows, columns>::Det() const {
  static_assert(rows == columns,
                "You can't take the determinant of a non-square matrix.");

  Matrix<rows - 1, columns - 1> MinorMatrix{};
  float determinant{0};
  for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
    // for odd indices the sign is negative
    float sign = (column_idx % 2 == 0) ? 1 : -1;
    determinant += sign * this->matrix[0][column_idx] *
                   this->MinorMatrix(MinorMatrix, 0, column_idx).Det();
  }

  return determinant;
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
    return 1e+10; // TODO: We should throw something here instead of failing
                  // quietly
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
std::array<float, columns> &Matrix<rows, columns>::
operator[](uint8_t row_index) {
  if (row_index > rows - 1) {
    return this->matrix[0]; // TODO: We should throw something here instead of
                            // failing quietly.
  }
  return this->matrix[row_index];
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &Matrix<rows, columns>::
operator=(const Matrix<rows, columns> &other) {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      this->matrix[row_idx][column_idx] = other.Get(row_idx, column_idx);
    }
  }
  // return a reference to ourselves so you can chain together these functions
  return *this;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> Matrix<rows, columns>::
operator+(const Matrix<rows, columns> &other) const {
  Matrix<rows, columns> buffer{};
  this->Add(other, buffer);
  return buffer;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> Matrix<rows, columns>::
operator-(const Matrix<rows, columns> &other) const {
  Matrix<rows, columns> buffer{};
  this->Sub(other, buffer);
  return buffer;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> Matrix<rows, columns>::
operator*(const Matrix<rows, columns> &other) const {
  Matrix<rows, columns> buffer{};
  this->Mult(other, buffer);
  return buffer;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> Matrix<rows, columns>::operator*(float scalar) const {
  Matrix<rows, columns> buffer{};
  this->Mult(scalar, buffer);
  return buffer;
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
void Matrix<rows, columns>::Fill(float value) {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      this->matrix[row_idx][column_idx] = value;
    }
  }
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::MatrixOfMinors(Matrix<rows, columns> &result) const {
  Matrix<rows - 1, columns - 1> MinorMatrix{};

  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      this->MinorMatrix(MinorMatrix, row_idx, column_idx);
      result[row_idx][column_idx] = MinorMatrix.Det();
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows - 1, columns - 1> &
Matrix<rows, columns>::MinorMatrix(Matrix<rows - 1, columns - 1> &result,
                                   uint8_t row_idx, uint8_t column_idx) const {
  std::array<float, (rows - 1) * (columns - 1)> subArray{};
  uint16_t array_idx{0};
  for (uint8_t row_iter{0}; row_iter < rows; row_iter++) {
    if (row_iter == row_idx) {
      continue;
    }
    for (uint8_t column_iter{0}; column_iter < columns; column_iter++) {
      if (column_iter == column_idx) {
        continue;
      }
      subArray[array_idx] = this->Get(row_iter, column_iter);
      array_idx++;
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
      result[column_iter][row_iter] = this->Get(row_iter, column_iter) * sign;
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
    result.Fill(1e+6);
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