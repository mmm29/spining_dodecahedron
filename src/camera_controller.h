#pragma once

#include "engine/controller.h"

class CameraController : public Controller {
public:
    void OnAttach(Engine *engine) override;

    void HandleMouseMovement(int x_offset, int y_offset);

    void Update(float ts) override;

private:
    Engine *engine_ = nullptr;

private:
    float mouse_sensitivity_ = 1.f;

    float moving_speed_ = 2.f;
    float fast_moving_speed_multiplier_ = 3.f;
};