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

private:
    std::shared_ptr<Mesh> mesh_;

    bool visible_ = true;
};