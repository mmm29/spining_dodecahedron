#pragma once

#include <memory>
#include <list>

#include "camera.h"
#include "view.h"
#include "draw_list.h"
#include "matrix.h"
#include "controller.h"

class Engine {
public:
    void Initialize(const ViewPort &viewport);

    void Draw();

    draw::DrawList *GetDrawList();

    std::shared_ptr<Camera> GetActiveCamera() const;

    void AttachController(const std::shared_ptr<Controller> &controller);

    void Update(float ts);

private:
    Matrix4 screen_space_matrix_;

private:
    // 3D projection
    std::shared_ptr<Camera> camera_;

    std::unique_ptr<View> view_;

private:
    // Drawing
    draw::DrawList draw_list_;

    std::list<std::shared_ptr<Controller>> controllers_;
};