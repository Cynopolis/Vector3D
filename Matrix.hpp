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
  void Add(const Matrix<rows, columns> &other,
           Matrix<rows, columns> &result) const;

  /**
   * @brief Element-wise subtract matrix
   * @param other the other matrix to subtract from this one
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  void Sub(const Matrix<rows, columns> &other,
           Matrix<rows, columns> &result) const;

  /**
   * @brief Matrix multiply the two matrices
   * @param other the other matrix to multiply into this one
   * @param result A buffer to store the result into
   */
  template <uint8_t other_columns>
  void Mult(const Matrix<rows, columns> &other,
            Matrix<columns, other_columns> &result) const;

  /**
   * @brief Multiply the matrix by a scalar
   * @param scalar the the scalar to multiply by
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  void Mult(float scalar, Matrix<rows, columns> &result) const;

  /**
   * @brief Invert this matrix
   * @param result A buffer to store the result into
   * @warning this is super slow! Only call it if you absolutely have to!!!
   */
  void Invert(Matrix<rows, columns> &result) const;

  /**
   * @brief Transpose this matrix
   * @param result A buffer to store the result into
   */
  void Transpose(Matrix<columns, rows> &result) const;

  /**
   * @brief Square this matrix
   * @param result A buffer to store the result into
   */
  void Square(Matrix<rows, columns> &result) const;

  /**
   * @return Get the determinant of the matrix
   */
  float Det() const;

  /**
   * @brief Calculate the eigenvalues for a square matrix
   * @param result a buffer to store the result into
   */
  void EigenValues(Matrix<rows, 1> &eigenvalues) const;

  /**
   * @brief Element-wise multiply the two matrices
   * @param other the other matrix to multiply into this one
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  void ElementMultiply(const Matrix<rows, columns> &other,
                       Matrix<rows, columns> &result) const;

  /**
   * @brief Element-wise divide the two matrices
   * @param other the other matrix to multiply into this one
   * @param result A buffer to store the result into
   * @note there is no problem if result == this
   */
  void ElementDivide(const Matrix<rows, columns> &other,
                     Matrix<rows, columns> &result) const;

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
    return this->matrix[row_index];
  }

  Matrix<rows, columns> &operator=(const Matrix<rows, columns> &other) {
    for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
      for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
        this->matrix[row_idx][column_idx] = other.Get(row_idx, column_idx);
      }
    }
  }

  /**
   * @brief Get a row from the matrix
   * @param row_index the row index to get
   * @param row a buffer to write the row into
   */
  void GetRow(uint8_t row_index, Matrix<1, columns> &row) const;

  /**
   * @brief Get a row from the matrix
   * @param column_index the row index to get
   * @param column a buffer to write the row into
   */
  void GetColumn(uint8_t column_index, Matrix<rows, 1> &column) const;

  /**
   * @brief Get the number of rows in this matrix
   */
  constexpr uint8_t GetRowSize() { return rows; }

  /**
   * @brief Get the number of columns in this matrix
   */
  constexpr uint8_t GetColumnSize() { return columns; }

  void ToString(std::string &stringBuffer) const;

private:
  /**
   * @brief take the dot product of the two vectors
   */
  template <uint8_t vector_size>
  static float dotProduct(const Matrix<1, vector_size> &vec1,
                          const Matrix<1, vector_size> &vec2);

  /**
   * @brief Set all elements in this matrix to zero
   */
  void zeroMatrix();

  void matrixOfMinors(Matrix<rows, columns> &result) const;

  void minorMatrix(Matrix<rows - 1, columns - 1> &result, uint8_t row_idx,
                   uint8_t column_idx) const;

  void adjugate(Matrix<rows, columns> &result) const;

  /**
   * @brief reduce the matrix so the sum of its elements equal 1
   * @param result a buffer to store the result into
   */
  void normalize(Matrix<rows, columns> &result) const;

  constexpr bool isSquare() { return rows == columns; }

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
void Matrix<rows, columns>::Add(const Matrix<rows, columns> &other,
                                Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) + other.Get(row_idx, column_idx);
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Sub(const Matrix<rows, columns> &other,
                                Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) - other.Get(row_idx, column_idx);
    }
  }
}

template <uint8_t rows, uint8_t columns>
template <uint8_t other_columns>
void Matrix<rows, columns>::Mult(const Matrix<rows, columns> &other,
                                 Matrix<columns, other_columns> &result) const {
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
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Mult(float scalar,
                                 Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] = this->Get(row_idx, column_idx) * scalar;
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Invert(Matrix<rows, columns> &result) const {
  // since all matrix sizes have to be statically specified at compile time we
  // can do this
  static_assert(rows == columns,
                "Your matrix isn't square and can't be inverted");

  // unfortunately we can't calculate this at compile time so we'll just reurn
  // zeros
  if (this->Det() < 0) {
    // you can't invert a matrix with a negative determinant
    result.zeroMatrix();
    return;
  }

  // TODO: This algorithm is really inneficient because of the matrix of minors.
  // We should make a different algorithm how to calculate the inverse:
  // https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html

  // calculate the matrix of minors
  Matrix<rows, columns> minors{};
  this->matrixOfMinors(minors);

  // now adjugate the matrix and save it in our output
  minors.adjugate(result);
  float determinant = this->Det();

  // scale the result by 1/determinant and we have our answer
  result.Mult(1 / determinant);
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Transpose(Matrix<columns, rows> &result) const {
  for (uint8_t column_idx{0}; column_idx < rows; column_idx++) {
    for (uint8_t row_idx{0}; row_idx < columns; row_idx++) {
      result[row_idx][column_idx] = this->Get(column_idx, row_idx);
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Square(Matrix<rows, columns> &result) const {
  static_assert(this->isSquare(), "You can't square an non-square matrix.");

  this->Mult(this, result);
}

template <uint8_t rows, uint8_t columns>
float Matrix<rows, columns>::Det() const {
  static_assert(this->isSquare(),
                "You can't take the determinant of a non-square matrix.");
  Matrix<1, columns> eigenValues{};
  this->EigenValues(eigenValues);

  float determinant{1};
  for (uint8_t i{0}; i < columns; i++) {
    determinant *= eigenValues.Get(0, i);
  }

  return determinant;
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::EigenValues(Matrix<rows, 1> &eigenvalues) const {
  static_assert(rows == columns,
                "Eigenvalues can only be computed for square matrices.");
  // I got this code from:
  // https://www.quora.com/What-is-the-C-code-for-finding-eigenvalues
  Matrix<rows, 1> v{};
  Matrix<rows, 1> Av{};
  Matrix<rows, 1> z{};

  float d = 0.0;
  float d_old = 0.0;
  constexpr float convergence_value{1e-6};
  constexpr uint16_t max_iterations{500};

  // Initialize v as a random vector
  for (int i = 0; i < rows; i++) {
    v[0][i] = rand() / RAND_MAX;
  }

  // run this loop until the eigenvalues converge or we give up
  for (uint16_t k{0}; k < max_iterations; k++) {
    /* Multiply A by v */
    for (int i = 0; i < rows; i++) {
      Av[0][i] = 0.0;
      for (int j = 0; j < rows; j++) {
        Av[0][i] += this->Get(0, i * rows + j) * v[0][j];
      }
    }

    // Calculate the eigenvalue and update v
    d_old = d;
    d = dot_product(v, Av, rows);
    for (int i = 0; i < rows; i++) {
      z[0][i] = Av[0][i] - d * v[0][i];
    }

    z.normalize(z);

    for (int i = 0; i < rows; i++) {
      v[0][i] = z[0][i];
    }

    /* Check for convergence */
    if (std::fabs(d - d_old) < convergence_value) {
      eigenvalues[0][k] = d;
      k++;
      d = 0.0;
      for (int i = 0; i < rows; i++) {
        v[0][i] = rand() / RAND_MAX;
      }
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::ElementMultiply(
    const Matrix<rows, columns> &other, Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) * other.Get(row_idx, column_idx);
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::ElementDivide(const Matrix<rows, columns> &other,
                                          Matrix<rows, columns> &result) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) / other.Get(row_idx, column_idx);
    }
  }
}

template <uint8_t rows, uint8_t columns>
float Matrix<rows, columns>::Get(uint8_t row_index,
                                 uint8_t column_index) const {
  return this->matrix[row_index][column_index];
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::GetRow(uint8_t row_index,
                                   Matrix<1, columns> &row) const {
  row = Matrix<1, columns>(this->matrix[row_index]);
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::GetColumn(uint8_t column_index,
                                      Matrix<rows, 1> &column) const {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    column[row_idx][0] = this->Get(row_idx, column_index);
  }
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
void Matrix<rows, columns>::zeroMatrix() {
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      this->matrix[row_idx][column_idx] = 0;
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::matrixOfMinors(
    Matrix<rows, columns> &result) const {
  Matrix<rows - 1, columns - 1> minorMatrix{};

  for (uint8_t row_idx{0}; row_idx < rows; row_idx++) {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++) {
      this->minorMatrix(minorMatrix, row_idx, column_idx);
      result[row_idx][column_idx] = minorMatrix.Det();
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::minorMatrix(Matrix<rows - 1, columns - 1> &result,
                                        uint8_t row_idx,
                                        uint8_t column_idx) const {
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
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::adjugate(Matrix<rows, columns> &result) const {
  for (uint8_t row_iter{0}; row_iter < rows; row_iter++) {
    for (uint8_t column_iter{0}; column_iter < columns; column_iter++) {
      float sign = ((row_iter + 1) % 2) == 0 ? -1 : 1;
      sign *= ((column_iter + 1) % 2) == 0 ? -1 : 1;
      result[row_iter][column_iter] = this->Get(row_iter, column_iter) * sign;
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::normalize(Matrix<rows, columns> &result) const {
  float sum{0};
  for (uint8_t column_idx{0}; column_idx < rows; column_idx++) {
    for (uint8_t row_idx{0}; row_idx < columns; row_idx++) {
      sum += this->Get(row_idx, column_idx);
    }
  }

  if (sum == 0) {
    // this wouldn't do anything anyways
    result.zeroMatrix();
    return;
  }

  for (uint8_t column_idx{0}; column_idx < rows; column_idx++) {
    for (uint8_t row_idx{0}; row_idx < columns; row_idx++) {
      result[row_idx][column_idx] = this->Get(row_idx, column_idx) / sum;
    }
  }
}