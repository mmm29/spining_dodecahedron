#pragma once

#include <memory>

#include "screen.h"
#include "camera.h"
#include "view.h"

class Engine {
public:
    void Initialize();

    bool Start();

private:
    std::unique_ptr<Screen> screen_;

    std::shared_ptr<Camera> camera_;

    std::unique_ptr<View> view_;
};