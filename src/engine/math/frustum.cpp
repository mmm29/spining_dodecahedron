#include <cassert>

#include "frustum.h"

#include "../constants.h"

// https://github.com/ezEngine/ezEngine/blob/dev/Code/Engine/Foundation/Math/Implementation/Frustum.cpp
// Gribb & Hartmann method - http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
void Frustum::SetFromModelViewProjection(const Matrix4 &model_view_projection0) {
    Vector4 planes[6];

    Matrix4 model_view_projection = model_view_projection0;
    for (size_t i = 0; i < 4; i++)
        model_view_projection[2][i] += model_view_projection[2][i] - model_view_projection[3][i];

    if (kHandedness == Handedness::kLeftHanded) {
        planes[kLeftPlane] = -model_view_projection.GetRow<4>(3) + model_view_projection.GetRow<4>(0);
        planes[kRightPlane] = -model_view_projection.GetRow<4>(3) - model_view_projection.GetRow<4>(0);
    } else {
        planes[kLeftPlane] = -model_view_projection.GetRow<4>(3) - model_view_projection.GetRow<4>(0);
        planes[kRightPlane] = -model_view_projection.GetRow<4>(3) + model_view_projection.GetRow<4>(0);
    }

    planes[kTopPlane] = -model_view_projection.GetRow<4>(3) + model_view_projection.GetRow<4>(1);
    planes[kBottomPlane] = -model_view_projection.GetRow<4>(3) - model_view_projection.GetRow<4>(1);
    planes[kNearPlane] = -model_view_projection.GetRow<4>(3) - model_view_projection.GetRow<4>(2);
    planes[kFarPlane] = -model_view_projection.GetRow<4>(3) + model_view_projection.GetRow<4>(2);

    // Normalize planes
    for (size_t plane_idx = 0; plane_idx < kPlanesCount; plane_idx++) {
        Vector3 plane_normal(planes[plane_idx][0], planes[plane_idx][1], planes[plane_idx][2]);
        const float length = plane_normal.GetLength();

        Plane &plane = planes_[plane_idx];
        plane.SetNormal(plane_normal / length);
        plane.SetDistance(-planes[plane_idx][3] / length);
    }
}

std::array<Vector3, Frustum::kCornersCount> Frustum::ComputeCornerPoints() const {
    std::array<Vector3, kCornersCount> corner_points;

    Plane::PlanesIntersectionPoint intersection;

    intersection = Plane::Intersect(planes_[kNearPlane], planes_[kTopPlane], planes_[kLeftPlane]);
    assert(intersection.exists);
    corner_points[kNearTopLeft] = intersection.point;

    intersection = Plane::Intersect(planes_[kNearPlane], planes_[kTopPlane], planes_[kRightPlane]);
    assert(intersection.exists);
    corner_points[kNearTopRight] = intersection.point;

    intersection = Plane::Intersect(planes_[kNearPlane], planes_[kBottomPlane], planes_[kLeftPlane]);
    assert(intersection.exists);
    corner_points[kNearBottomLeft] = intersection.point;

    intersection = Plane::Intersect(planes_[kNearPlane], planes_[kBottomPlane], planes_[kRightPlane]);
    assert(intersection.exists);
    corner_points[kNearBottomRight] = intersection.point;


    intersection = Plane::Intersect(planes_[kFarPlane], planes_[kTopPlane], planes_[kLeftPlane]);
    assert(intersection.exists);
    corner_points[kFarTopLeft] = intersection.point;

    intersection = Plane::Intersect(planes_[kFarPlane], planes_[kTopPlane], planes_[kRightPlane]);
    assert(intersection.exists);
    corner_points[kFarTopRight] = intersection.point;

    intersection = Plane::Intersect(planes_[kFarPlane], planes_[kBottomPlane], planes_[kLeftPlane]);
    assert(intersection.exists);
    corner_points[kFarBottomLeft] = intersection.point;

    intersection = Plane::Intersect(planes_[kFarPlane], planes_[kBottomPlane], planes_[kRightPlane]);
    assert(intersection.exists);
    corner_points[kFarBottomRight] = intersection.point;

    return corner_points;
}

void Frustum::Invert() {
    for (Plane &plane : planes_)
        plane.Flip();
}