#include "camera.h"
#include "graphics_utils.h"

Camera::Camera() : position_(0, 0, 0),
                   rotation_(0, 0, 0) {

}

void Camera::SetPosition(const Vector3 &position) {
    position_ = position;
}

void Camera::SetRotation(const Vector3 &rotation) {
    rotation_ = rotation;
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
    return CreateViewMatrix(position_, Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1));
}

Matrix4 Camera::GetProjectionMatrix(float aspect_ratio) const {
    return CreateProjectionMatrix(aspect_ratio, field_of_view_, near_z, far_z);
}