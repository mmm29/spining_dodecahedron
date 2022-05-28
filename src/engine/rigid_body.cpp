#include "rigid_body.h"

void RigidBody::SetMesh(const std::shared_ptr<Mesh> &mesh) {
    mesh_ = mesh;
}

std::shared_ptr<Mesh> RigidBody::GetMesh() const {
    return mesh_;
}

bool RigidBody::IsVisible() const {
    return visible_;
}

void RigidBody::SetVisible(bool visible) {
    visible_ = visible;
}

void RigidBody::SetColor(const Color &color) {
    color_ = color;
}

Color RigidBody::GetColor() const {
    return color_;
}

void RigidBody::SetRotationVelocity(const Vector2 &rotation_velocity) {
    rotation_velocity_ = rotation_velocity;
}

Vector2 RigidBody::GetRotationVelocity() const {
    return rotation_velocity_;
}