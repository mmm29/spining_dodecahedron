#pragma once

#include "vector.h"
#include "triangle.h"
#include "color.h"

class Drawer {
public:
    virtual ~Drawer() = default;

    virtual void DrawLine(const Vector2 &a, const Vector2 &b, const Color &color, float thickness = 1.f) = 0;

    virtual void DrawTriangle(const Triangle &triangle) = 0;
};