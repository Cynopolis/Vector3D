#include <cstdint>
#include <array>
#include <type_traits>

template <uint8_t rows, uint8_t columns>
class Matrix{
    public:
    /**
     * @brief Element-wise matrix addition
     * @param other the other matrix to add to this one
     * @param result A buffer to store the result into
     */
    void Add(const Matrix<rows, columns> & other, Matrix<rows, columns> & result) const;
    
    /**
     * @brief Element-wise subtract matrix
     * @param other the other matrix to subtract from this one
     * @param result A buffer to store the result into
     */
    void Subtract(const Matrix<rows, columns> & other, Matrix<rows, columns> & result) const;

    /**
     * @brief Matrix multiply the two matrices
     * @param other the other matrix to multiply into this one
     * @param result A buffer to store the result into
     */
    template <uint8_t other_columns>
    void Multiply(const Matrix<rows, columns> & other, Matrix<columns, other_columns> & result) const;
    
    /**
     * @brief Multiply the matrix by a scalar
     * @param scalar the the scalar to multiply by
     * @param result A buffer to store the result into
     */
    void Multiply(float scalar, Matrix<rows, columns> & result) const;
    
    /**
     * @brief Invert this matrix
     * @param result A buffer to store the result into
     */
    void Invert(Matrix<rows, columns> & result) const;

    /**
     * @brief Transpose this matrix
     * @param result A buffer to store the result into
     */
    void Transpose(Matrix<columns, rows> & result) const;

    /**
     * @brief Square this matrix
     * @param result A buffer to store the result into
     */
    void Square(Matrix<rows, columns> & result) const;

    /**
     * @return Get the determinant of the matrix
     */
    float Det();

    /**
     * @brief Element-wise multiply the two matrices
     * @param other the other matrix to multiply into this one
     * @param result A buffer to store the result into
     */
    void ElementMultiply(const Matrix<rows, columns> & other, Matrix<rows, columns> & result) const;

    /**
     * @brief Element-wise divide the two matrices
     * @param other the other matrix to multiply into this one
     * @param result A buffer to store the result into
     */
    void ElementDivide(const Matrix<rows, columns> & other, Matrix<rows, columns> & result) const;
    
    /**
     * @brief Get an element from the matrix
     * @param row the row index of the element
     * @param column the column index of the element
     * @return The value of the element you want to get
     */
    float & Get(uint8_t row_index, uint8_t column_index) const;

    /**
     * @brief Get a row from the matrix
     * @param row_index the row index to get
     * @param row a buffer to write the row into
     */
    void GetRow(uint8_t row_index, Matrix<rows, 1> & row) const;

    /**
     * @brief Get a row from the matrix
     * @param column_index the row index to get
     * @param column a buffer to write the row into
     */
    void GetColumn(uint8_t column_index, Matrix<1, columns> & column) const;

    /**
     * @brief Get the number of rows in this matrix
     */
    constexpr uint8_t GetRowSize(){return rows;}

    /**
     * @brief Get the number of columns in this matrix
     */
    constexpr uint8_t GetColumnSize(){return columns;}

    private:

    /**
     * @brief take the dot product of the two vectors
     */
    template <uint8_t vector_size>
    float dotProduct(const Matrix<vector_size, 1> & vec1, const Matrix<vector_size, 1> & vec2);
    
    /**
     * @brief Set all elements in this matrix to zero
     */
    void zeroMatrix();

    void matrixOfMinors(const Matrix<rows, columns> & input, Matrix<rows, columns> & result) const;

    void adjugate(const Matrix<rows, columns> & input, Matrix<rows, columns> & result) const;

    std::array<std::array<float, columns>, rows> matrix;
};

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Add(const Matrix<rows, columns> & other, Matrix<rows, columns> & result) const{
    for(uint8_t row{0}; row < rows; row++){
        for(uint8_t column{0}; column < columns; column++){
            result.Get(row, column) = this->Get(row, column) + other.Get(row, column);
        }
    }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Subtract(const Matrix<rows, columns> & other, Matrix<rows, columns> & result) const{
    for(uint8_t row{0}; row < rows; row++){
        for(uint8_t column{0}; column < columns; column++){
            result.Get(row, column) = this->Get(row, column) - other.Get(row, column);
        }
    }
}

template <uint8_t rows, uint8_t columns>
template <uint8_t other_columns>
void Matrix<rows, columns>::Multiply(const Matrix<rows, columns> & other, Matrix<columns, other_columns> & result) const{
    for(uint8_t row_idx{0}; row_idx < rows; row_idx++){
        for(uint8_t column_idx{0}; column_idx < columns; column_idx++){
            // get our row
            Matrix<rows, 1> this_row;
            this->GetRow(row_idx, this_row);
            // get the other matrices column
            Matrix<1, columns> other_column;
            other.GetColumn(column_idx, other_column);
            // transpose the other matrix's column
            Matrix<columns, 1> other_column_t;
            other_column.Transpose(other_column_t);

            // the result's index is equal to the dot product of these two vectors
            result.Get(row_idx, column_idx) = this->dotProduct(this_row, other_column_t);
        }
    }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Multiply(float scalar, Matrix<rows, columns> & result) const{
    for(uint8_t row_idx{0}; row_idx < rows; row_idx++){
        for(uint8_t column_idx{0}; column_idx < columns; column_idx++){
            result.Get(row_idx, column_idx) = this->Get(row_idx, column_idx) * scalar;
        }
    }
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Invert(Matrix<rows, columns> & result) const{
    // since all matrix sizes have to be statically specified at compile time we can do this
    static_assert(rows == columns, "Your matrix isn't square and can't be inverted");

    // unfortunately we can't calculate this at compile time so we'll just reurn zeros
    if(this->Det() < 0){
        // you can't invert a matrix with a negative determinant
        result.zeroMatrix();
        return;
    }

    // how to calculate the inverse: https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html

    // calculate the matrix of minors
    Matrix<rows, columns> minors{};
    this->matrixOfMinors(this, minors);

    // now adjugate the matrix and save it in our output
    this->adjugate(minors, result);
    float determinant = this->Det();

    // scale the result by 1/determinant and we have our answer
    result.Multiply(1/determinant);
}

template <uint8_t rows, uint8_t columns>
void Matrix<rows, columns>::Transpose(Matrix<columns, rows> & result) const{

}