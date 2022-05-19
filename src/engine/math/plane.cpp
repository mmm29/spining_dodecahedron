#include <cassert>

#include "plane.h"

Plane::Plane(const Vector3 &normal, const Vector3 &point) : normal_(normal),
                                                            distance_(normal_.Dot(point)) {
    assert(normal.IsNormalized());
}

Plane::Plane(const Vector3 &normal, float distance) : normal_(normal), distance_(distance) {
    assert(normal.IsNormalized());
}

const Vector3 &Plane::GetNormal() const {
    return normal_;
}

float Plane::DistanceTo(const Vector3 &p) const {
    return normal_.Dot(p) - distance_;
}

Plane::Intersection Plane::IntersectLine(const Vector3 &p1, const Vector3 &p2) const {
    return IntersectRay(p1, p2 - p1);
}

Plane::Intersection Plane::IntersectRay(const Vector3 &start, const Vector3 &direction) const {
    const float cos_alpha = normal_.Dot(direction);

    if (cos_alpha == 0)
        return {.orthogonal = true}; // Line is perpendicular to the plane.

    const float t = -DistanceTo(start) / cos_alpha;

    return {
            .orthogonal = false,
            .t = t,
            .point = start + direction * t
    };
}

bool Plane::IsInside(const Vector3 &point) const {
    return DistanceTo(point) >= 0;
}

bool Plane::IsOutside(const Vector3 &point) const {
    return DistanceTo(point) < 0;
}