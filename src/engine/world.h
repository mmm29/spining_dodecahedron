#pragma once

#include <list>
#include <memory>

#include "rigid_body.h"
#include "math/matrix.h"

class World {
public:
    World();

    void AddObject(const std::shared_ptr<RigidBody> &object);

    const std::list<std::shared_ptr<RigidBody>> &ListObjects() const;

    Matrix4 GetWorldMatrix() const;

private:
    Matrix4 world_matrix_;

    std::list<std::shared_ptr<RigidBody>> objects_;
};