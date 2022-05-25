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