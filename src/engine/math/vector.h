#pragma once

#include <cstdint>
#include <cmath>
#include <array>
#include <limits>

#include "math.h"

template<size_t Rows, size_t Cols>
class Matrix;

template<size_t Size>
class Vector {
public:
    Vector() : values_({}) {}

    Vector(std::initializer_list<float> il) : Vector(il, std::make_index_sequence<Size>()) {}

    template<class... Args>
    explicit Vector(Args... values) : values_({static_cast<float>(values)...}) {}

private:
    template<size_t... Is>
    Vector(std::initializer_list<float> il, std::index_sequence<Is...>) : values_{il.begin()[Is]...} {}

public:
    float &operator[](ptrdiff_t index) {
        return values_[index];
    }

    const float &operator[](ptrdiff_t index) const {
        return values_[index];
    }

    Matrix<Size, 1> AsMatrix() const {
        Matrix<Size, 1> result;

        for (size_t i = 0; i < Size; i++)
            result[i][0] = values_[i];

        return result;
    }

    Vector<Size> &operator*=(float scalar) {
        for (float &v : values_)
            v *= scalar;

        return *this;
    }

    Vector<Size> &operator/=(float scalar) {
        for (float &v : values_)
            v /= scalar;

        return *this;
    }

    Vector<Size> operator*(float scalar) const {
        Vector<Size> result = *this;
        result *= scalar;
        return result;
    }

    Vector<Size> operator/(float scalar) const {
        Vector<Size> result = *this;
        result /= scalar;
        return result;
    }

    Vector<Size> &operator+=(const Vector<Size> &other) {
        for (size_t i = 0; i < Size; i++)
            values_[i] += other.values_[i];

        return *this;
    }

    Vector<Size> &operator-=(const Vector<Size> &other) {
        for (size_t i = 0; i < Size; i++)
            values_[i] -= other.values_[i];

        return *this;
    }

    Vector<Size> operator+(const Vector<Size> &other) const {
        Vector<Size> result = *this;
        result += other;
        return result;
    }

    Vector<Size> operator-(const Vector<Size> &other) const {
        Vector<Size> result = *this;
        result -= other;
        return result;
    }

    Vector<Size> operator-() const {
        Vector<Size> result = *this;

        for (float &v : result.values_)
            v = -v;

        return result;
    }

    float GetLength() const {
        return sqrtf(GetLengthSquared());
    }

    float GetLengthSquared() const {
        float length = 0;

        for (const float &v : values_)
            length += v * v;

        return length;
    }

    Vector<Size> &Normalize() {
        const float length = GetLength();

        for (float &v : values_)
            v /= length;

        return *this;
    }

    bool IsNormalized(float epsilon = math::kSmallEpsilon) const {
        return math::IsEqual(GetLength(), 1.f, epsilon);
    }

    template<size_t DesiredSize>
    Vector<DesiredSize> AsVector() const {
        static_assert(DesiredSize >= Size);

        Vector<DesiredSize> result;

        for (size_t i = 0; i < Size; i++)
            result[i] = values_[i];

        return result;
    }

    float Dot(const Vector<Size> &rhs) const {
        float res = 0;

        for (size_t i = 0; i < Size; i++)
            res += values_[i] * rhs.values_[i];

        return res;
    }

    Vector<3> Cross(const Vector<3> &rhs) const {
        static_assert(Size == 3);

        return Vector<3>(values_[1] * rhs.values_[2] - values_[2] * rhs.values_[1],
                         values_[2] * rhs.values_[0] - values_[0] * rhs.values_[2],
                         values_[0] * rhs.values_[1] - values_[1] * rhs.values_[0]);
    }

    Vector<Size> operator*(const Vector<Size> &rhs) const {
        Vector<Size> result;

        for (size_t i = 0; i < Size; i++)
            result[i] = values_[i] * rhs[i];

        return result;
    }

    bool IsZero() const {
        for (const float &v : values_) {
            if (v != 0.f)
                return false;
        }

        return true;
    }

private:
    std::array<float, Size> values_;
};

using Vector2 = Vector<2>;
using Vector3 = Vector<3>;
using Vector4 = Vector<4>;