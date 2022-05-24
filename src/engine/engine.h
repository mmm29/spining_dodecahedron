#pragma once

#include <memory>
#include <vector>

#include "camera.h"
#include "view.h"
#include "math/matrix.h"
#include "controller.h"
#include "render/renderer.h"

// TODO: remove it
struct CameraInfo {
    std::shared_ptr<Camera> camera;

    bool show_viewing_frustum = false;
    Color viewing_frustum_color;
};

class Engine {
public:
    void Initialize(const ViewPort &viewport, std::shared_ptr<render::Renderer> &renderer);

    void Draw();

    std::shared_ptr<Camera> GetActiveCamera() const;

    void SetActiveCamera(const std::shared_ptr<Camera> &camera);

    std::shared_ptr<Camera> CreateCamera();

    void AttachController(const std::shared_ptr<Controller> &controller);

    void Update(float ts);

private:
    Matrix4 screen_space_matrix_;

private:
    std::shared_ptr<render::Renderer> renderer_;

    std::unique_ptr<View> view_;

private:
    std::vector<std::shared_ptr<Controller>> controllers_;
};