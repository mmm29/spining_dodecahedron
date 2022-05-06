#pragma once

struct ViewPort {
    ViewPort() = default;

    ViewPort(float width, float height) : width(width), height(height) {}

    float width;
    float height;

    float GetAspectRatio() const {
        return width / height;
    }
};