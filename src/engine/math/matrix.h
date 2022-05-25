#pragma once

#include <cstdint>
#include <utility>

#include "vector.h"

template<size_t Rows, size_t Cols>
class Matrix;

template<>
class Matrix<4, 4> {
public:
    Matrix() = default;

    Matrix(float _11, float _12, float _13, float _14,
           float _21, float _22, float _23, float _24,
           float _31, float _32, float _33, float _34,
           float _41, float _42, float _43, float _44)
            : _11(_11), _12(_12), _13(_13), _14(_14),
              _21(_21), _22(_22), _23(_23), _24(_24),
              _31(_31), _32(_32), _33(_33), _34(_34),
              _41(_41), _42(_42), _43(_43), _44(_44) {}

    Matrix(const Vector<4> &row1,
           const Vector<4> &row2,
           const Vector<4> &row3,
           const Vector<4> &row4)
            : rows{row1, row2, row3, row4} {}

public:
    void SetZero() {
        _11 = 0;
        _12 = 0;
        _13 = 0;
        _14 = 0;

        _21 = 0;
        _22 = 0;
        _23 = 0;
        _24 = 0;

        _31 = 0;
        _32 = 0;
        _33 = 0;
        _34 = 0;

        _41 = 0;
        _42 = 0;
        _43 = 0;
        _44 = 0;
    }

    void SetIdentity() {
        _11 = 1;
        _12 = 0;
        _13 = 0;
        _14 = 0;

        _21 = 0;
        _22 = 1;
        _23 = 0;
        _24 = 0;

        _31 = 0;
        _32 = 0;
        _33 = 1;
        _34 = 0;

        _41 = 0;
        _42 = 0;
        _43 = 1;
        _44 = 0;
    }

public:
    inline float &at(size_t row, size_t col) {
        return values[row][col];
    }

    inline const float &at(size_t row, size_t col) const {
        return values[row][col];
    }

    inline float *operator[](size_t row) {
        return values[row];
    }

    inline const float *operator[](size_t row) const {
        return values[row];
    }

    // Operators
    inline Matrix<4, 4> &operator*=(float scalar) {
        rows[0] *= scalar;
        rows[1] *= scalar;
        rows[2] *= scalar;
        rows[3] *= scalar;

        return *this;
    }

    inline Matrix<4, 4> &operator/=(float scalar) {
        rows[0] /= scalar;
        rows[1] /= scalar;
        rows[2] /= scalar;
        rows[3] /= scalar;

        return *this;
    }

    Matrix<4, 4> operator*(float scalar) const {
        return Matrix<4, 4>(rows[0] * scalar,
                            rows[1] * scalar,
                            rows[2] * scalar,
                            rows[3] * scalar);
    }

    Matrix<4, 4> operator/(float scalar) const {
        return Matrix<4, 4>(rows[0] / scalar,
                            rows[1] / scalar,
                            rows[2] / scalar,
                            rows[3] / scalar);
    }

    Matrix<4, 4> operator*(const Matrix<4, 4> &rhs) const {
        return Matrix<4, 4>(
                values[0][0] * rhs.values[0][0] + values[0][1] * rhs.values[1][0] + values[0][2] * rhs.values[2][0] +
                values[0][3] * rhs.values[3][0],
                values[0][0] * rhs.values[0][1] + values[0][1] * rhs.values[1][1] + values[0][2] * rhs.values[2][1] +
                values[0][3] * rhs.values[3][1],
                values[0][0] * rhs.values[0][2] + values[0][1] * rhs.values[1][2] + values[0][2] * rhs.values[2][2] +
                values[0][3] * rhs.values[3][2],
                values[0][0] * rhs.values[0][3] + values[0][1] * rhs.values[1][3] + values[0][2] * rhs.values[2][3] +
                values[0][3] * rhs.values[3][3],

                values[1][0] * rhs.values[0][0] + values[1][1] * rhs.values[1][0] + values[1][2] * rhs.values[2][0] +
                values[1][3] * rhs.values[3][0],
                values[1][0] * rhs.values[0][1] + values[1][1] * rhs.values[1][1] + values[1][2] * rhs.values[2][1] +
                values[1][3] * rhs.values[3][1],
                values[1][0] * rhs.values[0][2] + values[1][1] * rhs.values[1][2] + values[1][2] * rhs.values[2][2] +
                values[1][3] * rhs.values[3][2],
                values[1][0] * rhs.values[0][3] + values[1][1] * rhs.values[1][3] + values[1][2] * rhs.values[2][3] +
                values[1][3] * rhs.values[3][3],

                values[2][0] * rhs.values[0][0] + values[2][1] * rhs.values[1][0] + values[2][2] * rhs.values[2][0] +
                values[2][3] * rhs.values[3][0],
                values[2][0] * rhs.values[0][1] + values[2][1] * rhs.values[1][1] + values[2][2] * rhs.values[2][1] +
                values[2][3] * rhs.values[3][1],
                values[2][0] * rhs.values[0][2] + values[2][1] * rhs.values[1][2] + values[2][2] * rhs.values[2][2] +
                values[2][3] * rhs.values[3][2],
                values[2][0] * rhs.values[0][3] + values[2][1] * rhs.values[1][3] + values[2][2] * rhs.values[2][3] +
                values[2][3] * rhs.values[3][3],

                values[3][0] * rhs.values[0][0] + values[3][1] * rhs.values[1][0] + values[3][2] * rhs.values[2][0] +
                values[3][3] * rhs.values[3][0],
                values[3][0] * rhs.values[0][1] + values[3][1] * rhs.values[1][1] + values[3][2] * rhs.values[2][1] +
                values[3][3] * rhs.values[3][1],
                values[3][0] * rhs.values[0][2] + values[3][1] * rhs.values[1][2] + values[3][2] * rhs.values[2][2] +
                values[3][3] * rhs.values[3][2],
                values[3][0] * rhs.values[0][3] + values[3][1] * rhs.values[1][3] + values[3][2] * rhs.values[2][3] +
                values[3][3] * rhs.values[3][3]
        );
    }

    Vector<4> operator*(const Vector<4> &vec) const {
        return Vector<4>(values[0][0] * vec[0] + values[0][1] * vec[1] + values[0][2] * vec[2] + values[0][3] * vec[3],
                         values[1][0] * vec[0] + values[1][1] * vec[1] + values[1][2] * vec[2] + values[1][3] * vec[3],
                         values[2][0] * vec[0] + values[2][1] * vec[1] + values[2][2] * vec[2] + values[2][3] * vec[3],
                         values[3][0] * vec[0] + values[3][1] * vec[1] + values[3][2] * vec[2] + values[3][3] * vec[3]);
    }

    Vector<3> operator*(const Vector<3> &vec) const {
        return Vector<3>(values[0][0] * vec[0] + values[0][1] * vec[1] + values[0][2] * vec[2] + values[0][3],
                         values[1][0] * vec[0] + values[1][1] * vec[1] + values[1][2] * vec[2] + values[1][3],
                         values[2][0] * vec[0] + values[2][1] * vec[1] + values[2][2] * vec[2] + values[2][3]);
    }

    Matrix<4, 4> operator-() const {
        return Matrix<4, 4>(-_11, -_12, -_13, -_14,
                            -_21, -_22, -_23, -_24,
                            -_31, -_32, -_33, -_34,
                            -_41, -_42, -_43, -_44);
    }

    template<size_t Size>
    inline Vector<Size> GetRow(size_t row) const;

    template<size_t Size>
    inline void SetRow(size_t row, const Vector<Size> &values);

    template<size_t Size>
    inline Vector<Size> GetColumn(size_t col) const;

    template<size_t Size>
    inline void SetColumn(size_t column, const Vector<Size> &values);

    // Static methods
    static Matrix<4, 4> Identity() {
        Matrix<4, 4> result;
        result.SetIdentity();
        return result;
    }

    static Matrix<4, 4> Zero() {
        Matrix<4, 4> result;
        result.SetZero();
        return result;
    }

public:
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        Vector4 rows[4];
        float values[4][4];
    };
};

template<>
inline Vector<4> Matrix<4, 4>::GetRow(size_t row) const {
    return rows[row];
}

template<>
inline Vector<3> Matrix<4, 4>::GetRow(size_t row) const {
    return Vector<3>(values[row][0], values[row][1], values[row][2]);
}

template<>
inline void Matrix<4, 4>::SetRow(size_t row, const Vector<4> &values) {
    rows[row] = values;
}

template<>
inline void Matrix<4, 4>::SetRow(size_t row, const Vector<3> &values) {
    rows[row][0] = values[0];
    rows[row][1] = values[1];
    rows[row][2] = values[2];
}

template<>
inline Vector<4> Matrix<4, 4>::GetColumn(size_t col) const {
    return Vector<4>(values[0][col], values[1][col], values[2][col], values[3][col]);
}

template<>
inline Vector<3> Matrix<4, 4>::GetColumn(size_t col) const {
    return Vector<3>(values[0][col], values[1][col], values[2][col]);
}

template<>
inline void Matrix<4, 4>::SetColumn(size_t column, const Vector<4> &values) {
    this->values[0][column] = values[0];
    this->values[1][column] = values[1];
    this->values[2][column] = values[2];
    this->values[3][column] = values[3];
}

template<>
inline void Matrix<4, 4>::SetColumn(size_t column, const Vector<3> &values) {
    this->values[0][column] = values[0];
    this->values[1][column] = values[1];
    this->values[2][column] = values[2];
}

using Matrix4 = Matrix<4, 4>;