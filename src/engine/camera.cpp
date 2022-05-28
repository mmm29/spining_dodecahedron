#include <cassert>

#include "math/graphics_utils.h"

#include "camera.h"

void Camera::Initialize(const CameraInitializationParameters &params) {
    aspect_ratio_ = params.aspect_ratio;

    ResetCachedMatrices();
}

void Camera::SetFieldOfView(float field_of_view) {
    fov_ = field_of_view;
    ResetCachedMatrices();
}

float Camera::GetFieldOfView() const {
    return fov_;
}

float Camera::GetNearZ() const {
    return near_z_;
}

void Camera::SetNearZ(float near_z) {
    assert(near_z > 0);

    near_z_ = near_z;
    ResetCachedMatrices();
}

float Camera::GetFarZ() const {
    return far_z_;
}

void Camera::SetFarZ(float far_z) {
    assert(far_z > 0);

    far_z_ = far_z;
    ResetCachedMatrices();
}

void Camera::Update(float ts) {
    if (IsAttached())
        position_ = -direction_ * attach_distance_;
}

Matrix4 Camera::ComputeViewMatrix() const {
    return CreateViewMatrix(GetWorldPosition(),
                            rotation_matrix_.GetRow<3>(0),
                            rotation_matrix_.GetRow<3>(1),
                            rotation_matrix_.GetRow<3>(2));
}

Matrix4 Camera::ComputeProjectionMatrix() const {
    if (!is_projection_matrix_cached_) {
        cached_projection_matrix_ = CreateProjectionMatrix(aspect_ratio_, fov_, near_z_, far_z_);
        is_projection_matrix_cached_ = true;
    }

    return cached_projection_matrix_;
}

Matrix4 Camera::ComputeViewProjectionMatrix() const {
    return ComputeProjectionMatrix() * ComputeViewMatrix();
}

void Camera::ResetCachedMatrices() {
    is_projection_matrix_cached_ = false;
}

void Camera::SetAttachDistance(float attach_distance) {
    attach_distance_ = attach_distance;
}

float Camera::GetAttachDistance() const {
    return attach_distance_;
}