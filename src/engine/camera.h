#pragma once

#include <cmath>
#include <array>
#include <vector>

#include "vector.h"
#include "matrix.h"
#include "plane.h"

class Camera {
public:
    Camera();

    void Initialize(float aspect_ratio);

    void SetPosition(const Vector3 &position);

    const Vector2 &GetRotationAngles() const;

    void SetRotationAngles(const Vector2 &rotation_angles);

    const Vector3 &GetPosition() const;

    const Vector3 &GetDirection() const;

    // Field of view
    void SetFieldOfView(float field_of_view);

    float GetFieldOfView() const;

    Matrix4 GetViewMatrix() const;

    Matrix4 GetProjectionMatrix() const;

private:
    void UpdateRotation();

    void UpdateClippingPlanes();

private:
    Vector3 position_;

    Vector3 direction_;

    Matrix4 rotation_matrix_;

    Vector2 rotation_angles_;

    float fov_ = 55 * (M_PI / 180);

    float near_z = 0.1f;

    float far_z = 100.f;

    float aspect_ratio_;

    std::vector<Plane> clipping_planes_;
};