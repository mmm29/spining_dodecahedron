#include <cassert>

#include "engine.h"
#include "graphics_utils.h"

void Engine::Initialize(const ViewPort &viewport) {
    camera_ = std::make_shared<Camera>();

    view_ = std::make_unique<View>();
    view_->SetViewPort(viewport);
    view_->SetCamera(camera_);

    screen_space_matrix_ = CreateScreenSpaceMatrix(Vector2(viewport.width, viewport.height));
}

void Engine::Draw() {
    draw_list_.Clear();

    const auto to_screen = [&](const Vector3 &world_pos) -> Vector2 {
        Vector4 world_pos4(world_pos[0], world_pos[1], world_pos[2], 1.f);
        Vector4 res = screen_space_matrix_ * view_->GetViewData().view_projection_matrix * world_pos4;
        res /= res[3];
//        assert(res[2] >= 0 && res[2] <= 1 && "Z value must be normalized");
        return Vector2(res[0], res[1]);
    };

    view_->UpdateMatrices();

    {
        static const Vector2 net_corners[2] = {
                {-10.f, 0.1f},
                {10.f,  10.1f}
        };
        static const float net_y = -0.2f;
        static const size_t net_shares = 100;
        static const Vector2 net_share_step = (net_corners[1] - net_corners[0]) / net_shares;
        static const Color net_color = Color::Green();

        // Vertical lines
        for (size_t line = 0; line <= net_shares; line++) {
            const float x = net_corners[0][0] + net_share_step[0] * static_cast<float>(line);

            const Vector2 from = to_screen(Vector3(x, net_y, net_corners[0][1]));
            const Vector2 to = to_screen(Vector3(x, net_y, net_corners[1][1]));

            draw_list_.AddLine(from, to, net_color);
        }

        // Horizontal lines
        for (size_t line = 0; line <= net_shares; line++) {
            const float z = net_corners[0][1] + net_share_step[1] * static_cast<float>(line);

            const Vector2 from = to_screen(Vector3(net_corners[0][0], net_y, z));
            const Vector2 to = to_screen(Vector3(net_corners[1][0], net_y, z));

            draw_list_.AddLine(from, to, net_color);
        }
    }

    static const struct {
        Vector3 pos;
        Vector3 pos2;
        Color col;
    } l[] = {
            {{0.5, 0, 1}, {1,   0.5, 5}, Color::Black()},
            {{0.5, 0, 1}, {0.5, 0.2, 1}, Color::Red()}
    };

    for (auto &p : l)
        draw_list_.AddLine(to_screen(p.pos), to_screen(p.pos2), p.col);

    draw_list_.AddFilledTriangle(Vector2(200, 50), Vector2(100, 150), Vector2(300, 150), Color::Blue());

    Vector4 res = view_->GetViewData().projection_matrix * Vector4(0, -0.5, -1, 1);
    res /= res[3];
    res = screen_space_matrix_ * res;
    draw_list_.AddLine(Vector2(res[0], res[1]), Vector2(res[0], res[1] + 20), Color::Black());
}

draw::DrawList *Engine::GetDrawList() {
    return &draw_list_;
}

std::shared_ptr<Camera> Engine::GetActiveCamera() const {
    return camera_;
}