#pragma once

#include <cmath>
#include <array>

#include "vector.h"
#include "matrix.h"

class Camera {
public:
    Camera();

    void SetPosition(const Vector3 &position);

    const Vector2 &GetRotationAngles() const;

    void SetRotationAngles(const Vector2 &rotation_angles);

    const Vector3 &GetPosition() const;

    const Vector3 &GetRotation() const;

    // Field of view
    void SetFieldOfView(float field_of_view);

    float GetFieldOfView() const;

    Matrix4 GetViewMatrix() const;

    Matrix4 GetProjectionMatrix(float aspect_ratio) const;

private:
    void UpdateRotation();

private:
    Vector3 position_;

    Vector3 rotation_;

    Matrix4 rotation_matrix_;

    Vector2 rotation_angles_;

    float field_of_view_ = 55 * (M_PI / 180);

    float near_z = 0.1f;

    float far_z = 100.f;
};