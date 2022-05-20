#include <cassert>

#include "math.h"
#include "plane.h"

Plane::Plane() : normal_{}, distance_{} {}

Plane::Plane(const Vector3 &normal, const Vector3 &point) : normal_(normal),
                                                            distance_(normal_.Dot(point)) {
    assert(normal.IsNormalized());
}

Plane::Plane(const Vector3 &normal, float distance) : normal_(normal), distance_(distance) {
    assert(normal.IsNormalized());
}

void Plane::SetNormal(const Vector3 &normal) {
    assert(normal.IsNormalized());
    normal_ = normal;
}

void Plane::SetDistance(float distance) {
    distance_ = distance;
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

void Plane::Flip() {
    normal_ = -normal_;
    distance_ = -distance_;
}

Plane::PlanesIntersectionPoint Plane::Intersect(const Plane &p1, const Plane &p2, const Plane &p3) {
    const Vector3 &n1 = p1.normal_;
    const Vector3 &n2 = p2.normal_;
    const Vector3 &n3 = p3.normal_;

    assert(n1.IsNormalized());
    assert(n2.IsNormalized());
    assert(n3.IsNormalized());

    const Vector3 p2_cross_p3 = n2.Cross(n3);
    const float det = n1.Dot(p2_cross_p3);

    if (math::IsZero(det, math::kLargeEpsilon))
        return {.exists = false};

    PlanesIntersectionPoint intersection;

    intersection.exists = true;
    intersection.point = (p2_cross_p3 * p1.distance_ +
                          n3.Cross(n1) * p2.distance_ +
                          n1.Cross(n2) * p3.distance_) / det;

    return intersection;
}