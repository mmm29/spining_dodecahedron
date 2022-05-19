#pragma once

#include <cmath>

inline float Degree(float radians) {
    return radians * (180.f / static_cast<float>(M_PI));
}

inline float Radians(float degrees) {
    return degrees * (static_cast<float>(M_PI) / 180.f);
}