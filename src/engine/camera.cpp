#include "camera.h"
#include "graphics_utils.h"
#include "matrix_transform.h"

Camera::Camera() : position_(0, 0, 0),
                   rotation_angles_(0, 0) {
}

void Camera::Initialize(float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;

    UpdateRotation();
    UpdateClippingPlanes();
}

void Camera::SetPosition(const Vector3 &position) {
    position_ = position;
}

const Vector2 &Camera::GetRotationAngles() const {
    return rotation_angles_;
}

void Camera::SetRotationAngles(const Vector2 &rotation_angles) {
    // Strip value greater than 360 magnitude.
    const float yaw = std::fmod(rotation_angles[0], Radians(360));

    // Strip value greater than 360 magnitude.
    float pitch = std::fmod(rotation_angles[1], Radians(360));

    // Limit the pitch angle range to [-85 degree, 85 degree].
    pitch = std::fmax(std::fmin(pitch, Radians(85)), Radians(-85));

    rotation_angles_ = Vector2(yaw, pitch);
    UpdateRotation();
}

const Vector3 &Camera::GetPosition() const {
    return position_;
}

Vector3 Camera::GetDirection() const {
    return rotation_matrix_.GetRow<3>(2);
}

void Camera::SetFieldOfView(float field_of_view) {
    fov_ = field_of_view;
    UpdateClippingPlanes();
}

float Camera::GetFieldOfView() const {
    return fov_;
}

Matrix4 Camera::GetViewMatrix() const {
    return CreateViewMatrix(position_, rotation_matrix_.GetRow<3>(0),
                            rotation_matrix_.GetRow<3>(1),
                            -rotation_matrix_.GetRow<3>(2));
}

Matrix4 Camera::GetProjectionMatrix() const {
    return CreateProjectionMatrix(aspect_ratio_, fov_, near_z, far_z);
}

void Camera::UpdateRotation() {
    Matrix4 rotate_around_x = matrix::RotateAroundX(rotation_angles_[1]);
    Matrix4 rotate_around_y = matrix::RotateAroundY(-rotation_angles_[0]);
    rotation_matrix_ = rotate_around_x * rotate_around_y;
}

void Camera::UpdateClippingPlanes() {
    clipping_planes_.clear();
    clipping_planes_.reserve(6);

    // Far and near planes
    clipping_planes_.emplace_back(Vector3(0, 0, 1), Vector3(0, 0, near_z));
    clipping_planes_.emplace_back(Vector3(0, 0, 1), Vector3(0, 0, far_z));

    const float half_fov = fov_ / 2;
    const float cos_half_fov = std::cos(half_fov);
    const float sin_half_fov = std::sin(half_fov);

    // Up and down planes
    clipping_planes_.emplace_back(Vector3(0, cos_half_fov, sin_half_fov), Vector3());
    clipping_planes_.emplace_back(Vector3(0, -cos_half_fov, sin_half_fov), Vector3());

    const float half_fov_with_aspect = std::atan(aspect_ratio_ * std::tan(half_fov));
    const float cos_half_fov_with_aspect = std::cos(half_fov_with_aspect);
    const float sin_half_fov_with_aspect = std::sin(half_fov_with_aspect);

    // Left and right planes
    clipping_planes_.emplace_back(Vector3(-cos_half_fov_with_aspect, 0, sin_half_fov_with_aspect), Vector3());
    clipping_planes_.emplace_back(Vector3(cos_half_fov_with_aspect, 0, sin_half_fov_with_aspect), Vector3());
}