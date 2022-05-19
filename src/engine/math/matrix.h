#pragma once

#include <cstdint>
#include <array>

#include "vector.h"

template<size_t Rows, size_t Cols>
class Matrix {
public:
    Matrix() = default;

    Matrix(std::initializer_list<std::initializer_list<float>> il)
            : Matrix(il, std::make_index_sequence<Rows * Cols>()) {}

private:
    template<size_t... FlatIs>
    Matrix(std::initializer_list<std::initializer_list<float>> il,
           std::index_sequence<FlatIs...>)
            : values_{il.begin()[FlatIs / Rows].begin()[FlatIs % Cols]...} {}

public:
    void SetZero() {
        for (auto &r : values_) {
            for (float &v : r)
                v = 0.f;
        }
    }

    void SetIdentity() {
        static_assert(Rows == Cols && "Identity matrix can be created only with square matrix");

        for (size_t row_idx = 0; row_idx < Rows; row_idx++) {
            for (size_t col_idx = 0; col_idx < Cols; col_idx++)
                values_[row_idx][col_idx] = row_idx == col_idx ? 1.f : 0.f;
        }
    }

public:
    float &at(size_t row, size_t col) {
        return values_[row][col];
    }

    const float &at(size_t row, size_t col) const {
        return values_[row][col];
    }

    std::array<float, Cols> &operator[](size_t row) {
        return values_[row];
    }

    const std::array<float, Cols> &operator[](size_t row) const {
        return values_[row];
    }

    // Operators
    Matrix<Rows, Cols> &operator*=(float scalar) {
        for (auto &row : values_) {
            for (float &val : row)
                val *= scalar;
        }

        return *this;
    }

    Matrix<Rows, Cols> &operator/=(float scalar) {
        for (auto &row : values_) {
            for (float &val : row)
                val /= scalar;
        }

        return *this;
    }

    Matrix<Rows, Cols> operator*(float scalar) const {
        return (*this) *= scalar;
    }

    Matrix<Rows, Cols> operator/(float scalar) const {
        return (*this) /= scalar;
    }

    Matrix<Rows, Cols> operator*(const Matrix<Rows, Cols> &other) const {
        Matrix<Rows, Cols> result;
        result.SetZero();

        for (size_t outer_col = 0; outer_col < Cols; outer_col++) {
            for (size_t row = 0; row < Rows; row++) {
                for (size_t col = 0; col < Cols; col++)
                    result.at(row, outer_col) += values_[row][col] * other[col][outer_col];
            }
        }

        return result;
    }

    template<size_t OtherCols>
    Matrix<Rows, OtherCols> operator*(const Matrix<Rows, OtherCols> &other) const {
        Matrix<Rows, OtherCols> result;
        result.SetZero();

        for (size_t outer_col = 0; outer_col < OtherCols; outer_col++) {
            for (size_t row = 0; row < Rows; row++) {
                for (size_t col = 0; col < Cols; col++)
                    result.at(row, outer_col) += values_[row][col] * other[col][outer_col];
            }
        }

        return result;
    }

    Vector<Rows> operator*(const Vector<Cols> &vec) const {
        Vector<Rows> result;

        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++)
                result[row] += values_[row][col] * vec[col];
        }

        return result;
    }

    Matrix<Rows, Cols> operator-() const {
        Matrix<Rows, Cols> result = *this;

        for (auto &r : result.values_) {
            for (float &v : r)
                v = -v;
        }

        return result;
    }

    bool operator==(const Matrix<Rows, Cols> &rhs) const {
        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                if (values_[row][col] != rhs.values_[row][col])
                    return false;
            }
        }

        return true;
    }

    template<size_t Size>
    Vector<Size> GetRow(size_t row) const {
        static_assert(Size <= Cols);

        Vector<Size> result;

        for (size_t i = 0; i < Size; i++)
            result[i] = values_[row][i];

        return result;
    }

    template<size_t Size>
    void SetRow(size_t row, const Vector<Size> &values) {
        for (size_t i = 0; i < Size; i++)
            values_[row][i] = values[i];
    }

    template<size_t Size>
    Vector<Size> GetColumn(size_t col) const {
        static_assert(Size <= Rows);

        Vector<Size> result;

        for (size_t i = 0; i < Size; i++)
            result[i] = values_[i][col];

        return result;
    }

    template<size_t Size>
    void SetColumn(size_t column, const Vector<Size> &values) {
        for (size_t i = 0; i < Size; i++)
            values_[i][column] = values[i];
    }

    // Static methods
    static Matrix<Rows, Cols> Identity() {
        Matrix<Rows, Cols> result;
        result.SetIdentity();
        return result;
    }

    static Matrix<Rows, Cols> Zero() {
        Matrix<Rows, Cols> result;
        result.SetZero();
        return result;
    }

private:
    std::array<std::array<float, Cols>, Rows> values_;
};

using Matrix4 = Matrix<4, 4>;