#include "world.h"

World::World() {
    world_matrix_.SetIdentity();
}

void World::AddObject(const std::shared_ptr<RigidBody> &object) {
    objects_.emplace_back(object);
}

const std::list<std::shared_ptr<RigidBody>> &World::ListObjects() const {
    return objects_;
}

Matrix4 World::GetWorldMatrix() const {
    return world_matrix_;
}