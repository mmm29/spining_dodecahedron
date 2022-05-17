#pragma once

#include <vector>
#include <memory>

#include "object.h"
#include "plane.h"
#include "angle.h"
#include "world.h"

struct CameraInitializationParameters {
    float aspect_ratio;

    std::weak_ptr<World> world;
};

class Camera : public Object {
public:
    void Initialize(const CameraInitializationParameters &params);

public:
    void SetFieldOfView(float field_of_view);

    float GetFieldOfView() const;

public:
    Matrix4 GetViewMatrix() const;

    Matrix4 GetProjectionMatrix() const;

private:
    void UpdateClippingPlanes();

private:
    float fov_ = Radians(55);

    float near_z = 0.01f;

    float far_z = 100.f;

    float aspect_ratio_;

public:// TODO: remove
    std::vector<Plane> clipping_planes_;

private:
    std::weak_ptr<World> world_;
};