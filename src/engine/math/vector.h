#pragma once

#include <cstdint>
#include <cmath>
#include <array>

#include "math.h"

template<size_t Size>
struct Vector;

template<>
struct Vector<2> {
public:
    Vector() : values{} {}

    Vector(std::initializer_list<float> il) : values{il.begin()[0], il.begin()[1]} {}

    Vector(float x, float y) : x(x), y(y) {}

public:
    float &operator[](ptrdiff_t index) {
        return values[index];
    }

    const float &operator[](ptrdiff_t index) const {
        return values[index];
    }

    Vector<2> &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    Vector<2> &operator/=(float scalar) {
        x /= scalar;
        y /= scalar;

        return *this;
    }

    Vector<2> operator*(float scalar) const {
        return Vector<2>(x * scalar, y * scalar);
    }

    Vector<2> operator/(float scalar) const {
        return Vector<2>(x / scalar, y / scalar);
    }

    Vector<2> &operator+=(const Vector<2> &rhs) {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    Vector<2> &operator-=(const Vector<2> &rhs) {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    Vector<2> operator+(const Vector<2> &rhs) const {
        return Vector<2>(x + rhs.x, y + rhs.y);
    }

    Vector<2> operator-(const Vector<2> &rhs) const {
        return Vector<2>(x - rhs.x, y - rhs.y);
    }

    Vector<2> operator-() const {
        return Vector<2>(-x, -y);
    }

    float GetLength() const {
        return sqrtf(GetLengthSquared());
    }

    float GetLengthSquared() const {
        return x * x + y * y;
    }

    Vector<2> &Normalize() {
        const float length = GetLength();

        x /= length;
        y /= length;

        return *this;
    }

    bool IsNormalized(float epsilon = math::kSmallEpsilon) const {
        return math::IsEqual(GetLength(), 1.f, epsilon);
    }

    float Dot(const Vector<2> &rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    Vector<2> operator*(const Vector<2> &rhs) const {
        return Vector<2>(x * rhs.x, y * rhs.y);
    }

    bool IsZero() const {
        return math::IsZero(x) && math::IsZero(y);
    }

    Vector<3> AsVec3(float z = 1.0f) const;

    Vector<4> AsVec4(float z = 1.0f, float w = 1.0f) const;

public:
    union {
        struct {
            float x, y;
        };

        std::array<float, 2> values;
    };
};

template<>
struct Vector<3> {
public:
    Vector() : values{} {}

    Vector(std::initializer_list<float> il) : values{il.begin()[0], il.begin()[1], il.begin()[2]} {}

    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

public:
    float &operator[](ptrdiff_t index) {
        return values[index];
    }

    const float &operator[](ptrdiff_t index) const {
        return values[index];
    }

    Vector<3> &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;

        return *this;
    }

    Vector<3> &operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;

        return *this;
    }

    Vector<3> operator*(float scalar) const {
        return Vector<3>(x * scalar, y * scalar, z * scalar);
    }

    Vector<3> operator/(float scalar) const {
        return Vector<3>(x / scalar, y / scalar, z / scalar);
    }

    Vector<3> &operator+=(const Vector<3> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }

    Vector<3> &operator-=(const Vector<3> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return *this;
    }

    Vector<3> operator+(const Vector<3> &rhs) const {
        return Vector<3>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vector<3> operator-(const Vector<3> &rhs) const {
        return Vector<3>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vector<3> operator-() const {
        return Vector<3>(-x, -y, -z);
    }

    float GetLength() const {
        return sqrtf(GetLengthSquared());
    }

    float GetLengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vector<3> &Normalize() {
        const float length = GetLength();

        x /= length;
        y /= length;
        z /= length;

        return *this;
    }

    bool IsNormalized(float epsilon = math::kSmallEpsilon) const {
        return math::IsEqual(GetLength(), 1.f, epsilon);
    }

    float Dot(const Vector<3> &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    Vector<3> Cross(const Vector<3> &rhs) const {
        return Vector<3>(y * rhs.z - z * rhs.y,
                         z * rhs.x - x * rhs.z,
                         x * rhs.y - y * rhs.x);
    }

    Vector<3> operator*(const Vector<3> &rhs) const {
        return Vector<3>(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    bool IsZero() const {
        return math::IsZero(x) && math::IsZero(y) && math::IsZero(z);
    }

    Vector<2> AsVec2() const;

    Vector<4> AsVec4(float w = 1.0f) const;

public:
    union {
        struct {
            float x, y, z;
        };

        std::array<float, 3> values;
    };
};

template<>
struct Vector<4> {
public:
    Vector() : values{} {}

    Vector(std::initializer_list<float> il) : values{il.begin()[0], il.begin()[1], il.begin()[2], il.begin()[3]} {}

    Vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

public:
    float &operator[](ptrdiff_t index) {
        return values[index];
    }

    const float &operator[](ptrdiff_t index) const {
        return values[index];
    }

    Vector<4> &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;

        return *this;
    }

    Vector<4> &operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;

        return *this;
    }

    Vector<4> operator*(float scalar) const {
        return Vector<4>(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Vector<4> operator/(float scalar) const {
        return Vector<4>(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    Vector<4> &operator+=(const Vector<4> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;

        return *this;
    }

    Vector<4> &operator-=(const Vector<4> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;

        return *this;
    }

    Vector<4> operator+(const Vector<4> &rhs) const {
        return Vector<4>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vector<4> operator-(const Vector<4> &rhs) const {
        return Vector<4>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vector<4> operator-() const {
        return Vector<4>(-x, -y, -z, -w);
    }

    float GetLength() const {
        return sqrtf(GetLengthSquared());
    }

    float GetLengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    Vector<4> &Normalize() {
        const float length = GetLength();

        x /= length;
        y /= length;
        z /= length;
        w /= length;

        return *this;
    }

    bool IsNormalized(float epsilon = math::kSmallEpsilon) const {
        return math::IsEqual(GetLength(), 1.f, epsilon);
    }

    float Dot(const Vector<4> &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }

    Vector<4> operator*(const Vector<4> &rhs) const {
        return Vector<4>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    bool IsZero() const {
        return math::IsZero(x) && math::IsZero(y) && math::IsZero(z) && math::IsZero(w);
    }

    Vector<2> AsVec2() const;

    Vector<3> AsVec3() const;

public:
    union {
        struct {
            float x, y, z, w;
        };

        std::array<float, 4> values;
    };
};

using Vector2 = Vector<2>;
using Vector3 = Vector<3>;
using Vector4 = Vector<4>;