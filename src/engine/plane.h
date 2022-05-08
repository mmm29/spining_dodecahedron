#pragma once

#include "vector.h"

class Plane {
public:
    Plane(const Vector3 &normal, const Vector3 &point);

    const Vector3 &GetNormal() const;

    const Vector3 &GetPoint() const;

public:
    struct Intersection {
        bool exists;

        Vector3 point;

        float t;
    };

    Intersection Intersect(const Vector3 &p1, const Vector3 &p2) const;

    float DistanceTo(const Vector3 &p) const;

private:
    const Vector3 normal_;
    const Vector3 point_;
    const float normal_dot_point_;
};