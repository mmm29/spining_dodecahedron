#pragma once

#include <memory>

#include "mesh.h"
#include "object.h"

class RigidBody : public Object {
public:
    void SetMesh(const std::shared_ptr<Mesh> &mesh);

    std::shared_ptr<Mesh> GetMesh() const;

    bool IsVisible() const;

    void SetVisible(bool visible);

    void SetColor(const Color &color);

    Color GetColor() const;

    void SetRotationVelocity(const Vector2 &rotation_velocity);

    Vector2 GetRotationVelocity() const;

private:
    std::shared_ptr<Mesh> mesh_;

    Color color_;

    Vector2 rotation_velocity_ = Vector2::Zero();

    bool visible_ = true;
};