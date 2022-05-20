#pragma once

#include "vector.h"

class Plane {
public:
    Plane();

    Plane(const Vector3 &normal, const Vector3 &point);

    Plane(const Vector3 &normal, float distance);

    void SetNormal(const Vector3 &normal);

    void SetDistance(float distance);

    const Vector3 &GetNormal() const;

public:
    float DistanceTo(const Vector3 &p) const;

    struct Intersection {
        // Indicates whether the ray is orthogonal to the plane.
        bool orthogonal;

        // The intersection time describes at which multiple of the ray direction the ray hit the plane.
        float t;

        // The point where the ray hits the plane.
        Vector3 point;

    public:
        bool Exists() const {
            return !orthogonal && t >= 0 && t <= 1;
        }

        Vector3 &Point() {
            return point;
        }
    };

    Intersection IntersectLine(const Vector3 &p1, const Vector3 &p2) const;

    Intersection IntersectRay(const Vector3 &start, const Vector3 &direction) const;

    bool IsInside(const Vector3 &point) const;

    bool IsOutside(const Vector3 &point) const;

    void Flip();

    struct PlanesIntersectionPoint {
        bool exists;

        Vector3 point;
    };

    static PlanesIntersectionPoint Intersect(const Plane &p1, const Plane &p2, const Plane &p3);

private:
    Vector3 normal_;
    float distance_;
};