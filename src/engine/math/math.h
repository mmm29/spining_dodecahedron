#pragma once

namespace math {
    constexpr float kSmallEpsilon = 0.000001;

    constexpr float kDefaultEpsilon = 0.00001;

    constexpr float kLargeEpsilon = 0.0001;

    template<typename T>
    constexpr inline T Sign(T f) {
        return (f < 0 ? T(-1) : f > 0 ? T(1) : 0);
    }

    template<typename T>
    constexpr inline bool IsEqual(T lhs, T rhs, T epsilon = kDefaultEpsilon) {
        return ((rhs >= lhs - epsilon) && (rhs <= lhs + epsilon));
    }

    template<typename T>
    constexpr inline bool IsInRange(T value, T min, T max, T epsilon = kDefaultEpsilon) {
        if (min < max)
            return (value >= min - epsilon) && (value <= max + epsilon);
        else
            return (value >= max - epsilon) && (value <= min + epsilon);
    }

    template<typename T>
    constexpr inline bool IsZero(T f, T epsilon = kDefaultEpsilon) {
        return (f >= -epsilon) && (f <= epsilon);
    }
}