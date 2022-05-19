#include "math/angle.h"
#include "math/matrix_transform.h"
#include "object.h"

Object::Object() {
    UpdateTransformationMatrix();
}

void Object::SetPosition(const Vector3 &position) {
    position_ = position;
}

const Vector3 &Object::GetPosition() const {
    return position_;
}

void Object::SetRotationAngles(const Vector2 &rotation_angles) {
    // Strip value greater than 360 magnitude.
    const float yaw = std::fmod(rotation_angles[0], Radians(360));

    // Strip value greater than 360 magnitude.
    float pitch = std::fmod(rotation_angles[1], Radians(360));

    // Limit the pitch angle range to [-85 degree, 85 degree].
    pitch = std::max(std::min(pitch, Radians(85)), Radians(-85));

    rotation_angles_ = Vector2(yaw, pitch);

    UpdateTransformationMatrix();
}

const Vector2 &Object::GetRotationAngles() const {
    return rotation_angles_;
}

Vector3 Object::GetDirection() const {
    return direction_;
}

void Object::UpdateTransformationMatrix() {
    Matrix4 rotate_around_x = matrix::RotateAroundX(rotation_angles_[1]);
    Matrix4 rotate_around_y = matrix::RotateAroundY(-rotation_angles_[0]);
    transformation_matrix_ = rotate_around_x * rotate_around_y;

    direction_ = transformation_matrix_.GetRow<3>(2);
}