#pragma once

#include <list>
#include <memory>

#include "object.h"

class World {
public:
    void AddObject(const std::shared_ptr<Object> &object);

    const std::list<std::shared_ptr<Object>> &ListObjects() const;

private:
    std::list<std::shared_ptr<Object>> objects_;
};