#pragma once

#include <memory>
#include <list>

#include "world.h"
#include "camera.h"
#include "view.h"
#include "math/matrix.h"
#include "controller.h"
#include "render/renderer.h"
#include "settings.h"

// TODO: remove it
struct CameraInfo {
    std::shared_ptr<Camera> camera;

    bool show_viewing_frustum = false;
    Color viewing_frustum_color;
};

class Engine {
public:
    void Initialize(const ViewPort &viewport, std::shared_ptr<render::Renderer> &renderer);

    void Update(float ts);

    void Draw();

public:
    std::shared_ptr<Camera> GetActiveCamera() const;

    void SetActiveCamera(const std::shared_ptr<Camera> &camera);

    std::shared_ptr<Camera> CreateCamera();

public:
    void AttachController(const std::shared_ptr<Controller> &controller);

public:
    std::shared_ptr<World> GetWorld() const;

public:
    Settings *AccessSettings();

    void SetDefaultSettings();
private:
    Matrix4 screen_space_matrix_;

private:
    std::shared_ptr<World> world_;

    std::shared_ptr<render::Renderer> renderer_;

    std::unique_ptr<View> view_;

private:
    std::list<std::shared_ptr<Controller>> controllers_;

private:
    Settings settings_;
};