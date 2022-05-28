#include <cassert>

#include "math/angle.h"
#include "math/matrix_transform.h"
#include "object.h"

Object::Object() {
    position_.SetZero();
    rotation_angles_.SetZero();
    direction_.SetZero();
    rotation_matrix_.SetIdentity();

    UpdateRotationMatrix();
}

void Object::SetWorldPosition(const Vector3 &position) {
    if (attached_to_)
        position_ = position - attached_to_->GetWorldPosition();
    else
        position_ = position;
}

void Object::Move(const Vector3 &offset) {
    position_ += offset;
}

Vector3 Object::GetWorldPosition() const {
    if (attached_to_)
        return attached_to_->GetWorldPosition() + position_;

    return position_;
}

void Object::SetRelativePosition(const Vector3 &position) {
    position_ = position;
}

Vector3 Object::GetRelativePosition() const {
    return position_;
}

void Object::SetRotationAngles(const Vector2 &rotation_angles) {
    // Strip value greater than 360 magnitude.
    const float yaw = std::fmod(rotation_angles[0], Radians(360));

    // Strip value greater than 360 magnitude.
    float pitch = std::fmod(rotation_angles[1], Radians(360));

    rotation_angles_ = Vector2(yaw, pitch);

    UpdateRotationMatrix();
}

Vector2 Object::GetRotationAngles() const {
    return rotation_angles_;
}

Vector3 Object::GetDirectionForward() const {
    return direction_;
}

Matrix4 Object::GetModelMatrix() const {
    return matrix::Translate(position_) * rotation_matrix_;
}

void Object::AttachTo(const std::shared_ptr<Object> &object) {
    assert(object.get() != this);

    position_ = GetWorldPosition() - object->GetWorldPosition();

    if (attached_to_)
        position_ -= attached_to_->GetWorldPosition();

    attached_to_ = object;
}

void Object::Detach() {
    assert(attached_to_);

    position_ += attached_to_->GetWorldPosition();
    attached_to_ = nullptr;
}

bool Object::IsAttached() const {
    return attached_to_ != nullptr;
}

void Object::UpdateRotationMatrix() {
    Matrix4 rotate_around_x = matrix::RotateAroundX(rotation_angles_[1]);
    Matrix4 rotate_around_y = matrix::RotateAroundY(-rotation_angles_[0]);
    rotation_matrix_ = rotate_around_x * rotate_around_y;

    direction_ = rotation_matrix_.GetRow<3>(2);
}