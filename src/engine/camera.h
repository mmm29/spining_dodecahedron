#pragma once

#include <vector>
#include <memory>

#include "object.h"
#include "math/plane.h"
#include "math/angle.h"
#include "world.h"

struct CameraInitializationParameters {
    float aspect_ratio;

    std::weak_ptr<World> world;
};

class Camera : public Object {
public:
    void Initialize(const CameraInitializationParameters &params);

public:
    void SetFieldOfView(float field_of_view);

    float GetFieldOfView() const;

public:
    Matrix4 ComputeViewMatrix() const;

    Matrix4 ComputeProjectionMatrix() const;

private:
    void ResetCachedMatrices();

private:
    float fov_ = Radians(55);

    float near_z = 0.01f;

    float far_z = 100.f;

    float aspect_ratio_;

private:
    std::weak_ptr<World> world_;

private:
    // Cached matrices
    mutable bool is_projection_matrix_cached_ = false;
    mutable Matrix4 cached_projection_matrix_;
};