#pragma once

#include <array>

#include "plane.h"
#include "matrix.h"

class Frustum {
public:
    enum PlaneType {
        kNearPlane,
        kFarPlane,
        kLeftPlane,
        kRightPlane,
        kTopPlane,
        kBottomPlane,

        kPlanesCount
    };

    enum Corner {
        kNearTopLeft,
        kNearTopRight,
        kNearBottomLeft,
        kNearBottomRight,
        kFarTopLeft,
        kFarTopRight,
        kFarBottomLeft,
        kFarBottomRight,

        kCornersCount
    };

    void SetFromModelViewProjection(const Matrix4 &model_view_projection);

    std::array<Vector3, kCornersCount> ComputeCornerPoints() const;

    void Invert();

public: // TODO: make it private
    std::array<Plane, kPlanesCount> planes_;
};