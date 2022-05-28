#pragma once

#include <cstdint>
#include <cmath>
#include <cstddef>
#include <initializer_list>

#include "math.h"

template<size_t Size>
struct Vector;

template<>
struct Vector<2> {
public:
    Vector() = default;

    Vector(std::initializer_list<float> il) : values{il.begin()[0], il.begin()[1]} {}

    Vector(float x, float y) : x(x), y(y) {}

public:
    void SetZero() {
        x = 0;
        y = 0;
    }

    static Vector<2> Zero() {
        Vector<2> result;
        result.SetZero();
        return result;
    }

    inline float &operator[](ptrdiff_t index) {
        return values[index];
    }

    inline const float &operator[](ptrdiff_t index) const {
        return values[index];
    }

    inline Vector<2> &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    inline Vector<2> &operator/=(float scalar) {
        x /= scalar;
        y /= scalar;

        return *this;
    }

    inline Vector<2> operator*(float scalar) const {
        return Vector<2>(x * scalar, y * scalar);
    }

    inline Vector<2> operator/(float scalar) const {
        return Vector<2>(x / scalar, y / scalar);
    }

    inline Vector<2> &operator+=(const Vector<2> &rhs) {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    inline Vector<2> &operator-=(const Vector<2> &rhs) {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    inline Vector<2> operator+(const Vector<2> &rhs) const {
        return Vector<2>(x + rhs.x, y + rhs.y);
    }

    inline Vector<2> operator-(const Vector<2> &rhs) const {
        return Vector<2>(x - rhs.x, y - rhs.y);
    }

    inline Vector<2> operator-() const {
        return Vector<2>(-x, -y);
    }

    inline float GetLength() const {
        return sqrtf(GetLengthSquared());
    }

    inline float GetLengthSquared() const {
        return x * x + y * y;
    }

    inline Vector<2> &Normalize() {
        const float length = GetLength();

        x /= length;
        y /= length;

        return *this;
    }

    inline Vector<2> GetNormalized() const {
        const float length = GetLength();

        return Vector<2>(x / length, y / length);
    }

    inline bool IsNormalized(float epsilon = math::kSmallEpsilon) const {
        return math::IsEqual(GetLength(), 1.f, epsilon);
    }

    inline float Dot(const Vector<2> &rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    inline Vector<2> operator*(const Vector<2> &rhs) const {
        return Vector<2>(x * rhs.x, y * rhs.y);
    }

    inline bool IsZero() const {
        return math::IsZero(x) && math::IsZero(y);
    }

    inline Vector<3> AsVec3(float z = 1.0f) const;

    inline Vector<4> AsVec4(float z = 1.0f, float w = 1.0f) const;

public:
    union {
        struct {
            float x, y;
        };

        float values[2];
    };
};

template<>
struct Vector<3> {
public:
    Vector() = default;

    Vector(std::initializer_list<float> il) : values{il.begin()[0], il.begin()[1], il.begin()[2]} {}

    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

public:
    void SetZero() {
        x = 0;
        y = 0;
        z = 0;
    }

    static Vector<3> Zero() {
        Vector<3> result;
        result.SetZero();
        return result;
    }

    inline float &operator[](ptrdiff_t index) {
        return values[index];
    }

    inline const float &operator[](ptrdiff_t index) const {
        return values[index];
    }

    inline Vector<3> &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;

        return *this;
    }

    inline Vector<3> &operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;

        return *this;
    }

    inline Vector<3> operator*(float scalar) const {
        return Vector<3>(x * scalar, y * scalar, z * scalar);
    }

    inline Vector<3> operator/(float scalar) const {
        return Vector<3>(x / scalar, y / scalar, z / scalar);
    }

    inline Vector<3> &operator+=(const Vector<3> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }

    inline Vector<3> &operator-=(const Vector<3> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return *this;
    }

    inline Vector<3> operator+(const Vector<3> &rhs) const {
        return Vector<3>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    inline Vector<3> operator-(const Vector<3> &rhs) const {
        return Vector<3>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    inline Vector<3> operator-() const {
        return Vector<3>(-x, -y, -z);
    }

    inline float GetLength() const {
        return sqrtf(GetLengthSquared());
    }

    inline float GetLengthSquared() const {
        return x * x + y * y + z * z;
    }

    inline Vector<3> &Normalize() {
        const float length = GetLength();

        x /= length;
        y /= length;
        z /= length;

        return *this;
    }

    inline Vector<3> GetNormalized() const {
        const float length = GetLength();

        return Vector<3>(x / length, y / length, z / length);
    }

    inline bool IsNormalized(float epsilon = math::kSmallEpsilon) const {
        return math::IsEqual(GetLength(), 1.f, epsilon);
    }

    inline float Dot(const Vector<3> &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    inline Vector<3> Cross(const Vector<3> &rhs) const {
        return Vector<3>(y * rhs.z - z * rhs.y,
                         z * rhs.x - x * rhs.z,
                         x * rhs.y - y * rhs.x);
    }

    inline Vector<3> operator*(const Vector<3> &rhs) const {
        return Vector<3>(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    inline bool IsZero() const {
        return math::IsZero(x) && math::IsZero(y) && math::IsZero(z);
    }

    inline Vector<2> AsVec2() const;

    inline Vector<4> AsVec4(float w = 1.0f) const;

public:
    union {
        struct {
            float x, y, z;
        };

        float values[3];
    };
};

template<>
struct Vector<4> {
public:
    Vector() = default;

    Vector(std::initializer_list<float> il) : values{il.begin()[0], il.begin()[1], il.begin()[2], il.begin()[3]} {}

    Vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

public:
    void SetZero() {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    static Vector<4> Zero() {
        Vector<4> result;
        result.SetZero();
        return result;
    }

    inline float &operator[](ptrdiff_t index) {
        return values[index];
    }

    inline const float &operator[](ptrdiff_t index) const {
        return values[index];
    }

    inline Vector<4> &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;

        return *this;
    }

    inline Vector<4> &operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;

        return *this;
    }

    inline Vector<4> operator*(float scalar) const {
        return Vector<4>(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    inline Vector<4> operator/(float scalar) const {
        return Vector<4>(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    inline Vector<4> &operator+=(const Vector<4> &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;

        return *this;
    }

    inline Vector<4> &operator-=(const Vector<4> &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;

        return *this;
    }

    inline Vector<4> operator+(const Vector<4> &rhs) const {
        return Vector<4>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    inline Vector<4> operator-(const Vector<4> &rhs) const {
        return Vector<4>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    inline Vector<4> operator-() const {
        return Vector<4>(-x, -y, -z, -w);
    }

    inline float GetLength() const {
        return sqrtf(GetLengthSquared());
    }

    inline float GetLengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    inline Vector<4> &Normalize() {
        const float length = GetLength();

        x /= length;
        y /= length;
        z /= length;
        w /= length;

        return *this;
    }

    inline Vector<4> GetNormalized() const {
        const float length = GetLength();

        return Vector<4>(x / length, y / length, z / length, w / length);
    }

    inline bool IsNormalized(float epsilon = math::kSmallEpsilon) const {
        return math::IsEqual(GetLength(), 1.f, epsilon);
    }

    inline float Dot(const Vector<4> &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }

    inline Vector<4> operator*(const Vector<4> &rhs) const {
        return Vector<4>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    inline bool IsZero() const {
        return math::IsZero(x) && math::IsZero(y) && math::IsZero(z) && math::IsZero(w);
    }

    inline Vector<2> AsVec2() const;

    inline Vector<3> AsVec3() const;

public:
    union {
        struct {
            float x, y, z, w;
        };

        float values[4];
    };
};

Vector<3> Vector<2>::AsVec3(float z) const {
    return Vector<3>(x, y, z);
}

Vector<4> Vector<2>::AsVec4(float z, float w) const {
    return Vector<4>(x, y, z, w);
}

Vector<2> Vector<3>::AsVec2() const {
    return Vector<2>(x, y);
}

Vector<4> Vector<3>::AsVec4(float w) const {
    return Vector<4>(x, y, z, w);
}

Vector<2> Vector<4>::AsVec2() const {
    return Vector<2>(x, y);
}

Vector<3> Vector<4>::AsVec3() const {
    return Vector<3>(x, y, z);
}

using Vector2 = Vector<2>;
using Vector3 = Vector<3>;
using Vector4 = Vector<4>;