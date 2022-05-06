#pragma once

#include <cmath>

#include "vector.h"
#include "matrix.h"

class Camera {
public:
    Camera();

    void SetPosition(const Vector3 &position);

    void SetRotation(const Vector3 &rotation);

    const Vector3 &GetPosition() const;

    const Vector3 &GetRotation() const;

    // Field of view
    void SetFieldOfView(float field_of_view);

    float GetFieldOfView() const;

    Matrix4 GetViewMatrix() const;

    Matrix4 GetProjectionMatrix(float aspect_ratio) const;

private:
    Vector3 position_;

    Vector3 rotation_;

    float field_of_view_ = M_PI / 16;

    float near_z = 0.1f;

    float far_z = 100.f;
};