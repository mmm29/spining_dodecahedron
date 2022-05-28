#pragma once

#include <vector>
#include <memory>

#include "object.h"
#include "math/plane.h"
#include "math/angle.h"
#include "world.h"

struct CameraInitializationParameters {
    float aspect_ratio;
};

class Camera : public Object {
public:
    void Initialize(const CameraInitializationParameters &params);

public:
    void SetFieldOfView(float field_of_view);

    float GetFieldOfView() const;

public:
    float GetNearZ() const;

    void SetNearZ(float near_z);

public:
    float GetFarZ() const;

    void SetFarZ(float far_z);

public:
    void Update(float ts);

public:
    Matrix4 ComputeViewMatrix() const;

    Matrix4 ComputeProjectionMatrix() const;

    Matrix4 ComputeViewProjectionMatrix() const;

    void SetAttachDistance(float attach_distance);

    float GetAttachDistance() const;

private:
    void ResetCachedMatrices();

private:
    float fov_ = Radians(60);

    float near_z_ = 0.1f;

    float far_z_ = 100.f;

    float aspect_ratio_;

private:
    float attach_distance_ = 10.f;

private:
    // Cached matrices
    mutable bool is_projection_matrix_cached_ = false;
    mutable Matrix4 cached_projection_matrix_;
};