#ifdef MATRIX_H_ // since the .cpp file has to be included by the .hpp file this
                 // will evaluate to true
#include "Matrix.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <type_traits>
#include <cstring>

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns>::Matrix(float value)
{
  this->Fill(value);
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns>::Matrix(const std::array<float, rows * columns> &array)
{
  this->setMatrixToArray(array);
}

template <uint8_t rows, uint8_t columns>
template <typename... Args>
Matrix<rows, columns>::Matrix(Args... args)
{
  constexpr uint16_t arraySize{static_cast<uint16_t>(rows) *
                               static_cast<uint16_t>(columns)};

  std::initializer_list<float> initList{static_cast<float>(args)...};
  // choose whichever buffer size is smaller for the copy length
  uint32_t minSize =
      std::min(arraySize, static_cast<uint16_t>(initList.size()));
  memcpy(this->matrix.begin(), initList.begin(), minSize * sizeof(float));
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Identity()
{
  this->Fill(0);
  for (uint8_t idx{0}; idx < rows; idx++)
  {
    this->matrix[idx * columns + idx] = 1;
  }
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns>::Matrix(const Matrix<rows, columns> &other)
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      this->matrix[row_idx * columns + column_idx] =
          other.Get(row_idx, column_idx);
    }
  }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::setMatrixToArray(
    const std::array<float, rows * columns> &array)
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      uint16_t array_idx =
          static_cast<uint16_t>(row_idx) * static_cast<uint16_t>(columns) +
          static_cast<uint16_t>(column_idx);
      if (array_idx < array.size())
      {
        this->matrix[row_idx * columns + column_idx] = array[array_idx];
      }
      else
      {
        this->matrix[row_idx * columns + column_idx] = 0;
      }
    }
  }
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Add(const Matrix<rows, columns> &other,
                           Matrix<rows, columns> &result) const
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) + other.Get(row_idx, column_idx);
    }
  }
  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Sub(const Matrix<rows, columns> &other,
                           Matrix<rows, columns> &result) const
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) - other.Get(row_idx, column_idx);
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
template <uint8_t other_columns>
Matrix<rows, other_columns> &
Matrix<rows, columns>::Mult(const Matrix<columns, other_columns> &other,
                            Matrix<rows, other_columns> &result) const
{
  // allocate some buffers for all of our dot products
  Matrix<1, columns> this_row;
  Matrix<columns, 1> other_column;

  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    // get our row
    this->GetRow(row_idx, this_row);
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      // get the other matrix'ss column
      other.GetColumn(column_idx, other_column);

      // the result's index is equal to the dot product of these two vectors
      result[row_idx][column_idx] =
          Matrix<rows, columns>::DotProduct(this_row, other_column.Transpose());
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Mult(float scalar, Matrix<rows, columns> &result) const
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      result[row_idx][column_idx] = this->Get(row_idx, column_idx) * scalar;
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns>
Matrix<rows, columns>::Invert() const
{
  // since all matrix sizes have to be statically specified at compile time we
  // can do this
  static_assert(rows == columns,
                "Your matrix isn't square and can't be inverted");

  Matrix<rows, columns> result{};
  // unfortunately we can't calculate this at compile time so we'll just reurn
  // zeros
  float determinant{this->Det()};
  if (determinant == 0)
  {
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
Matrix<columns, rows>
Matrix<rows, columns>::Transpose() const
{
  Matrix<columns, rows> result{};
  for (uint8_t column_idx{0}; column_idx < rows; column_idx++)
  {
    for (uint8_t row_idx{0}; row_idx < columns; row_idx++)
    {
      result[row_idx][column_idx] = this->Get(column_idx, row_idx);
    }
  }

  return result;
}

// explicitly define the determinant for a 2x2 matrix because it is definitely
// the fastest way to calculate a 2x2 matrix determinant
template <>
inline float Matrix<0, 0>::Det() const { return 1e+6; }
template <>
inline float Matrix<1, 1>::Det() const { return this->matrix[0]; }
template <>
inline float Matrix<2, 2>::Det() const
{
  return this->matrix[0] * this->matrix[3] - this->matrix[1] * this->matrix[2];
}

template <uint8_t rows, uint8_t columns>
float Matrix<rows, columns>::Det() const
{
  static_assert(rows == columns,
                "You can't take the determinant of a non-square matrix.");

  Matrix<rows - 1, columns - 1> MinorMatrix{};
  float determinant{0};
  for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
  {
    // for odd indices the sign is negative
    float sign = (column_idx % 2 == 0) ? 1 : -1;
    determinant += sign * this->matrix[column_idx] *
                   this->MinorMatrix(MinorMatrix, 0, column_idx).Det();
  }

  return determinant;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::ElementMultiply(const Matrix<rows, columns> &other,
                                       Matrix<rows, columns> &result) const
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) * other.Get(row_idx, column_idx);
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::ElementDivide(const Matrix<rows, columns> &other,
                                     Matrix<rows, columns> &result) const
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      result[row_idx][column_idx] =
          this->Get(row_idx, column_idx) / other.Get(row_idx, column_idx);
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
float Matrix<rows, columns>::Get(uint8_t row_index,
                                 uint8_t column_index) const
{
  if (row_index > rows - 1 || column_index > columns - 1)
  {
    return 1e+10; // TODO: We should throw something here instead of failing
                  // quietly
  }
  return this->matrix[row_index * columns + column_index];
}

template <uint8_t rows, uint8_t columns>
Matrix<1, columns> &
Matrix<rows, columns>::GetRow(uint8_t row_index,
                              Matrix<1, columns> &row) const
{
  memcpy(&(row[0]), this->matrix.begin() + row_index * columns,
         columns * sizeof(float));

  return row;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, 1> &
Matrix<rows, columns>::GetColumn(uint8_t column_index,
                                 Matrix<rows, 1> &column) const
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    column[row_idx][0] = this->Get(row_idx, column_index);
  }

  return column;
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::ToString(std::string &stringBuffer) const
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    stringBuffer += "|";
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      stringBuffer +=
          std::to_string(this->matrix[row_idx * columns + column_idx]);
      if (column_idx != columns - 1)
      {
        stringBuffer += "\t";
      }
    }
    stringBuffer += "|\n";
  }
}

template <uint8_t rows, uint8_t columns>
std::array<float, columns> &Matrix<rows, columns>::
operator[](uint8_t row_index)
{
  if (row_index > rows - 1)
  {
    // TODO: We should throw something here instead of failing quietly.
    row_index = 0;
  }
  // cursed reinterpret_cast that will help us fake having a nested array when
  // we really don't
  return *reinterpret_cast<std::array<float, columns> *>(
      &(this->matrix[row_index * columns]));
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &Matrix<rows, columns>::
operator=(const Matrix<rows, columns> &other)
{
  memcpy(this->matrix.begin(), other.matrix.begin(),
         rows * columns * sizeof(float));

  // return a reference to ourselves so you can chain together these functions
  return *this;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> Matrix<rows, columns>::
operator+(const Matrix<rows, columns> &other) const
{
  Matrix<rows, columns> buffer{};
  this->Add(other, buffer);
  return buffer;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> Matrix<rows, columns>::
operator-(const Matrix<rows, columns> &other) const
{
  Matrix<rows, columns> buffer{};
  this->Sub(other, buffer);
  return buffer;
}

template <uint8_t rows, uint8_t columns>
template <uint8_t other_columns>
Matrix<rows, other_columns> Matrix<rows, columns>::
operator*(const Matrix<columns, other_columns> &other) const
{
  Matrix<rows, other_columns> buffer{};
  this->Mult(other, buffer);
  return buffer;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> Matrix<rows, columns>::operator*(float scalar) const
{
  Matrix<rows, columns> buffer{};
  this->Mult(scalar, buffer);
  return buffer;
}

template <uint8_t rows, uint8_t columns>
template <uint8_t vector_size>
float Matrix<rows, columns>::DotProduct(const Matrix<1, vector_size> &vec1,
                                        const Matrix<1, vector_size> &vec2)
{
  float sum{0};
  for (uint8_t i{0}; i < vector_size; i++)
  {
    sum += vec1.Get(0, i) * vec2.Get(0, i);
  }

  return sum;
}

template <uint8_t rows, uint8_t columns>
template <uint8_t vector_size>
float Matrix<rows, columns>::DotProduct(const Matrix<vector_size, 1> &vec1,
                                        const Matrix<vector_size, 1> &vec2)
{
  float sum{0};
  for (uint8_t i{0}; i < vector_size; i++)
  {
    sum += vec1.Get(i, 0) * vec2.Get(i, 0);
  }

  return sum;
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Fill(float value)
{
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      this->matrix[row_idx * columns + column_idx] = value;
    }
  }
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::MatrixOfMinors(Matrix<rows, columns> &result) const
{
  Matrix<rows - 1, columns - 1> MinorMatrix{};

  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      this->MinorMatrix(MinorMatrix, row_idx, column_idx);
      result[row_idx][column_idx] = MinorMatrix.Det();
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows - 1, columns - 1> &
Matrix<rows, columns>::MinorMatrix(Matrix<rows - 1, columns - 1> &result,
                                   uint8_t row_idx, uint8_t column_idx) const
{
  std::array<float, (rows - 1) * (columns - 1)> subArray{};
  uint16_t array_idx{0};
  for (uint8_t row_iter{0}; row_iter < rows; row_iter++)
  {
    if (row_iter == row_idx)
    {
      continue;
    }
    for (uint8_t column_iter{0}; column_iter < columns; column_iter++)
    {
      if (column_iter == column_idx)
      {
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
Matrix<rows, columns>::adjugate(Matrix<rows, columns> &result) const
{
  for (uint8_t row_iter{0}; row_iter < rows; row_iter++)
  {
    for (uint8_t column_iter{0}; column_iter < columns; column_iter++)
    {
      float sign = ((row_iter + 1) % 2) == 0 ? -1 : 1;
      sign *= ((column_iter + 1) % 2) == 0 ? -1 : 1;
      result[column_iter][row_iter] = this->Get(row_iter, column_iter) * sign;
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
Matrix<rows, columns> &
Matrix<rows, columns>::Normalize(Matrix<rows, columns> &result) const
{
  float sum{0};
  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      float val{this->Get(row_idx, column_idx)};
      sum += val * val;
    }
  }

  if (sum == 0)
  {
    // this wouldn't do anything anyways
    result.Fill(1e+6);
    return result;
  }

  sum = sqrt(sum);

  for (uint8_t row_idx{0}; row_idx < rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < columns; column_idx++)
    {
      result[row_idx][column_idx] = this->Get(row_idx, column_idx) / sum;
    }
  }

  return result;
}

template <uint8_t rows, uint8_t columns>
template <uint8_t sub_rows, uint8_t sub_columns, uint8_t row_offset, uint8_t column_offset>
Matrix<sub_rows, sub_columns> Matrix<rows, columns>::SubMatrix() const
{
  // static assert that sub_rows + row_offset <= rows
  // static assert that sub_columns + column_offset <= columns
  static_assert(sub_rows + row_offset <= rows,
                "The submatrix you're trying to get is out of bounds (rows)");
  static_assert(sub_columns + column_offset <= columns,
                "The submatrix you're trying to get is out of bounds (columns)");

  Matrix<sub_rows, sub_columns> buffer{};
  for (uint8_t row_idx{0}; row_idx < sub_rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < sub_columns; column_idx++)
    {
      buffer[row_idx][column_idx] =
          this->Get(row_idx + row_offset, column_idx + column_offset);
    }
  }
  return buffer;
}

template <uint8_t rows, uint8_t columns>
template <uint8_t sub_rows, uint8_t sub_columns, uint8_t row_offset, uint8_t column_offset>
void Matrix<rows, columns>::SetSubMatrix(const Matrix<sub_rows, sub_columns> &sub_matrix)
{
  static_assert(sub_rows + row_offset <= rows,
                "The submatrix you're trying to set is out of bounds (rows)");
  static_assert(sub_columns + column_offset <= columns,
                "The submatrix you're trying to set is out of bounds (columns)");

  for (uint8_t row_idx{0}; row_idx < sub_rows; row_idx++)
  {
    for (uint8_t column_idx{0}; column_idx < sub_columns; column_idx++)
    {
      this->matrix[(row_idx + row_offset) * columns + column_idx + column_offset] = sub_matrix.Get(row_idx, column_idx);
    }
  }
}

#endif // MATRIX_H_