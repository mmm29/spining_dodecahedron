#include <cassert>

#include <SFML/Window/Keyboard.hpp>

#include "engine/engine.h"

#include "camera_controller.h"

void CameraController::OnAttach(Engine *engine) {
    engine_ = engine;
}

void CameraController::HandleMouseMovement(int x_offset, int y_offset) {
    if (!engine_)
        return;

    std::shared_ptr<Camera> camera = engine_->GetActiveCamera();
    assert(camera && "No active camera.");

    Vector2 rotation_angeles_change(x_offset, -y_offset);

    static const float kDefaultCameraRotation = Radians(3.f / 25.f);
    rotation_angeles_change *= kDefaultCameraRotation * mouse_sensitivity_;

    Vector2 rotation_angles = camera->GetRotationAngles() + rotation_angeles_change;

    // Limit the pitch angle range to [-85 degree, 85 degree].
    rotation_angles[1] = std::max(std::min(rotation_angles[1], Radians(85)), Radians(-85));

    camera->SetRotationAngles(rotation_angles);
}

void CameraController::Update(float ts) {
    assert(engine_ && "OnAttach has not been called.");

    std::shared_ptr<Camera> camera = engine_->GetActiveCamera();
    assert(camera && "No active camera.");

    Vector3 direction = camera->GetDirectionForward();
    assert(direction.IsNormalized() && "Rotation vector is not normalized.");

    Vector3 offset = Vector3::Zero();

    // Forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        offset += direction;

    // Backward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        offset -= direction;

    // Left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        offset -= Vector3(direction[2], 0, -direction[0]).Normalize();

    // Right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        offset += Vector3(direction[2], 0, -direction[0]).Normalize();

    // Up
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        offset += Vector3(0, 1, 0);

    // Down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        offset -= Vector3(0, 1, 0);

    if (!offset.IsZero()) {
        offset *= moving_speed_ * ts;

        // Fast
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            offset *= fast_moving_speed_multiplier_;

        camera->Move(offset);
    }
}