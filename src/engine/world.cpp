#include "world.h"

void World::AddObject(const std::shared_ptr<Object> &object) {
    objects_.emplace_back(object);
}

const std::list<std::shared_ptr<Object>> &World::ListObjects() const {
    return objects_;
}