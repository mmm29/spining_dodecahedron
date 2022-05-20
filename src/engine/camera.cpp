#include "math/graphics_utils.h"

#include "camera.h"

void Camera::Initialize(const CameraInitializationParameters &params) {
    aspect_ratio_ = params.aspect_ratio;
    world_ = params.world;

    ResetCachedMatrices();
}

void Camera::SetFieldOfView(float field_of_view) {
    fov_ = field_of_view;

    ResetCachedMatrices();
}

float Camera::GetFieldOfView() const {
    return fov_;
}

Matrix4 Camera::ComputeViewMatrix() const {
    return CreateViewMatrix(position_, transformation_matrix_.GetRow<3>(0),
                            transformation_matrix_.GetRow<3>(1),
                            transformation_matrix_.GetRow<3>(2));
}

Matrix4 Camera::ComputeProjectionMatrix() const {
    if (!is_projection_matrix_cached_) {
        cached_projection_matrix_ = CreateProjectionMatrix(aspect_ratio_, fov_, near_z, far_z);
        is_projection_matrix_cached_ = true;
    }

    return cached_projection_matrix_;
}

void Camera::ResetCachedMatrices() {
    is_projection_matrix_cached_ = false;
}