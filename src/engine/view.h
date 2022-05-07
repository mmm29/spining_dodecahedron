#pragma once

#include <memory>

#include "camera.h"
#include "matrix.h"
#include "viewport.h"

struct ViewData {
    Matrix4 view_matrix;
    Matrix4 projection_matrix;
    Matrix4 view_projection_matrix;
};

class View {
public:
    void SetViewPort(const ViewPort &viewport);

    const ViewPort &GetViewPort() const;

    // Camera
    void SetCamera(const std::shared_ptr<Camera> &camera);

    void RemoveCamera();

    const std::shared_ptr<Camera> &GetCamera() const;

    bool IsCameraAttached() const;

public:
    void UpdateMatrices();

    const ViewData& GetViewData() const;

private:
    ViewPort viewport_;
    float viewport_aspect_ratio_;

    std::shared_ptr<Camera> camera_;

private:
    ViewData data_;
};