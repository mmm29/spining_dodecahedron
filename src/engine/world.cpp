#include "world.h"

World::World() {
    world_matrix_.SetIdentity();
}

void World::AddObject(const std::shared_ptr<Object> &object) {
    objects_.emplace_back(object);
}

const std::list<std::shared_ptr<Object>> &World::ListObjects() const {
    return objects_;
}

Matrix4 World::GetWorldMatrix() const {
    return world_matrix_;
}