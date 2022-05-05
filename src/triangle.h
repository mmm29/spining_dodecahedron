#pragma once

#include "vector.h"
#include "color.h"

struct Triangle {
    Triangle(const Vector2 &a, const Vector2 &b, const Vector2 &c, const Color &color, float outline = 0.f)
            : positions{a, b, c}, color(color), outline(outline) {}

    Vector2 positions[3];

    Color color;

    float outline;
};