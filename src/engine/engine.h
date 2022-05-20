#pragma once

#include <memory>
#include <list>

#include "camera.h"
#include "view.h"
#include "draw_list.h"
#include "math/matrix.h"
#include "controller.h"

// TODO: remove it
struct CameraInfo {
    std::shared_ptr<Camera> camera;

    bool show_viewing_frustum = false;
    Color viewing_frustum_color;
};

class Engine {
public:
    void Initialize(const ViewPort &viewport);

    void Draw();

    draw::DrawList *GetDrawList();

    std::shared_ptr<Camera> GetActiveCamera() const;

    void SetActiveCamera(const std::shared_ptr<Camera>& camera);

    std::shared_ptr<Camera> CreateCamera();

    void AttachController(const std::shared_ptr<Controller> &controller);

    void Update(float ts);

private:
    Matrix4 screen_space_matrix_;

private:
    // 3D projection
    std::unique_ptr<View> view_;

private:
    // Drawing
    draw::DrawList draw_list_;

    std::list<std::shared_ptr<Controller>> controllers_;
};