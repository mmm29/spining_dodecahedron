#include "plane.h"

Plane::Plane(const Vector3 &normal, const Vector3 &point) : normal_(normal), point_(point),
                                                            normal_dot_point_(normal_.Dot(point_)) {}

const Vector3 &Plane::GetNormal() const {
    return normal_;
}

const Vector3 &Plane::GetPoint() const {
    return point_;
}

Plane::Intersection Plane::Intersect(const Vector3 &p1, const Vector3 &p2) const {
    const Vector3 direction = p2 - p1;
    const float normal_dot_direction = normal_.Dot(direction);

    if (normal_dot_direction == 0)
        return {false};

    const float normal_dot_p1 = normal_.Dot(p1);
    const float t = -(normal_dot_p1 + normal_dot_point_) / normal_dot_direction;

    return {true, p1 + direction * t, t};
}

float Plane::DistanceTo(const Vector3 &p) const {
    return normal_.Dot(p) - normal_dot_point_;
}