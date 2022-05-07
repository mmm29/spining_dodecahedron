#include "camera.h"
#include "graphics_utils.h"
#include "matrix_transform.h"

Camera::Camera() : position_(0, 0, 0),
                   rotation_angles_(0, 0) {
    UpdateRotation();
}

void Camera::SetPosition(const Vector3 &position) {
    position_ = position;
}

const Vector2 &Camera::GetRotationAngles() const {
    return rotation_angles_;
}

void Camera::SetRotationAngles(const Vector2 &rotation_angles) {
    rotation_angles_ = rotation_angles;
    UpdateRotation();
}

const Vector3 &Camera::GetPosition() const {
    return position_;
}

const Vector3 &Camera::GetRotation() const {
    return rotation_;
}

void Camera::SetFieldOfView(float field_of_view) {
    field_of_view_ = field_of_view;
}

float Camera::GetFieldOfView() const {
    return field_of_view_;
}

Matrix4 Camera::GetViewMatrix() const {
    return CreateViewMatrix(position_, rotation_matrix_.GetRow<3>(0).Normalize(),
                            rotation_matrix_.GetRow<3>(1).Normalize(),
                            -rotation_matrix_.GetRow<3>(2).Normalize());
}

Matrix4 Camera::GetProjectionMatrix(float aspect_ratio) const {
    return CreateProjectionMatrix(aspect_ratio, field_of_view_, near_z, far_z);
}

void Camera::UpdateRotation() {
    Matrix4 rotate_around_x = matrix::RotateAroundX(rotation_angles_[1]);
    Matrix4 rotate_around_y = matrix::RotateAroundY(-rotation_angles_[0]);
    rotation_matrix_ = rotate_around_x * rotate_around_y;

    rotation_ = Vector3(-rotation_matrix_[0][2], rotation_matrix_[2][1], rotation_matrix_[2][2]).Normalize();
}