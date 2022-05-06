#pragma once

#include <cstdint>
#include <cmath>
#include <array>
#include <limits>

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

    float GetLength() const {
        return sqrtf(GetLengthSquared());
    }

    float GetLengthSquared() const {
        float length = 0;

        for (const float &v : values_)
            length += v * v;

        return length;
    }

    void Normalize() {
        const float length = GetLength();

        for (float &v : values_)
            v /= length;
    }

    bool IsNormalized() const {
        return std::fabs(GetLengthSquared() - 1) < std::numeric_limits<float>::epsilon();
    }

    template<size_t DesiredSize>
    Vector<DesiredSize> AsVector() const {
        static_assert(DesiredSize >= Size);

        Vector<DesiredSize> result;

        for (size_t i = 0; i < Size; i++)
            result[i] = values_[i];

        return result;
    }

private:
    std::array<float, Size> values_;
};

using Vector2 = Vector<2>;
using Vector3 = Vector<3>;
using Vector4 = Vector<4>;