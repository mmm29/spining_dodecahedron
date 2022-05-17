#include "graphics_utils.h"

#include "camera.h"

void Camera::Initialize(const CameraInitializationParameters &params) {
    aspect_ratio_ = params.aspect_ratio;
    world_ = params.world;

    UpdateClippingPlanes();
}

void Camera::SetFieldOfView(float field_of_view) {
    fov_ = field_of_view;
    UpdateClippingPlanes();
}

float Camera::GetFieldOfView() const {
    return fov_;
}

Matrix4 Camera::GetViewMatrix() const {
    return CreateViewMatrix(position_, transformation_matrix_.GetRow<3>(0),
                            transformation_matrix_.GetRow<3>(1),
                            -transformation_matrix_.GetRow<3>(2));
}

Matrix4 Camera::GetProjectionMatrix() const {
    return CreateProjectionMatrix(aspect_ratio_, fov_, near_z, far_z);
}

void Camera::UpdateClippingPlanes() {
    clipping_planes_.clear();
    clipping_planes_.reserve(6);

    // Far and near planes
    clipping_planes_.emplace_back(Vector3(0, 0, -1), Vector3(0, 0, near_z));
    clipping_planes_.emplace_back(Vector3(0, 0, -1), Vector3(0, 0, far_z));

    const float half_fov = fov_ / 2;
    const float cos_half_fov = std::cos(half_fov);
    const float sin_half_fov = std::sin(half_fov);

    // Up and down planes
    clipping_planes_.emplace_back(-Vector3(0, cos_half_fov, sin_half_fov), Vector3());
    clipping_planes_.emplace_back(-Vector3(0, -cos_half_fov, sin_half_fov), Vector3());

    const float half_fov_with_aspect = std::atan(aspect_ratio_ * std::tan(half_fov));
    const float cos_half_fov_with_aspect = std::cos(half_fov_with_aspect);
    const float sin_half_fov_with_aspect = std::sin(half_fov_with_aspect);

    // Left and right planes
    clipping_planes_.emplace_back(-Vector3(-cos_half_fov_with_aspect, 0, sin_half_fov_with_aspect), Vector3());
    clipping_planes_.emplace_back(-Vector3(cos_half_fov_with_aspect, 0, sin_half_fov_with_aspect), Vector3());
}