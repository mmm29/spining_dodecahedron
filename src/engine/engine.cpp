#include <cassert>

#include "engine.h"
#include "math/graphics_utils.h"

void Engine::Initialize(const ViewPort &viewport) {
    camera_ = std::make_shared<Camera>();
    camera_->Initialize(CameraInitializationParameters{
            .aspect_ratio = viewport.GetAspectRatio(),
            .world = std::weak_ptr<World>()
    });

//    camera_->SetPosition(Vector3(0, 0, 0.31898));

    view_ = std::make_unique<View>();
    view_->SetViewPort(viewport);
    view_->SetCamera(camera_);

    screen_space_matrix_ = CreateScreenSpaceMatrix(Vector2(viewport.width, viewport.height));
}

void Engine::Draw() {
    draw_list_.Clear();

    const auto to_screen = [&](const Vector3 &world_pos) -> Vector2 {
//        assert(world_pos[2] > -std::numeric_limits<float>::epsilon());

        Vector4 res(world_pos[0], world_pos[1], world_pos[2], 1.f);
//        res = view_->GetViewData().view_matrix * res;
        res = view_->GetViewData().projection_matrix * res;
//        assert(res[0] >= -1 && res[0] <= 1 && "X value is not in range [-1,1]");
//        assert(res[1] >= -1 && res[1] <= 1 && "Y value is not in range [-1,1]");

        res = screen_space_matrix_ * res;
        assert(res[3] > 0 && "W value must be greater than zero");

        res /= res[3];
//        assert(res[2] >= 0 && res[2] <= 1 && "Z value must be normalized");

        return Vector2(res[0], res[1]);
    };

    {
        static Plane plane(Vector3(0, 0, -1), Vector3(0, 0, 0.1));
        Plane::Intersection intersection = plane.Intersect(Vector3(-1, 0, 1.31897998), Vector3(1, 0, -1.68102002));
        assert(intersection.exists &&
               std::fabs(intersection.point[2] - plane.GetPoint()[2]) < std::numeric_limits<float>::epsilon());
    }

    const auto draw_line = [&](Vector3 from, Vector3 to, const Color &color) {
        Vector4 from4(from[0], from[1], from[2], 1.f);
        from4 = view_->GetViewData().view_matrix * from4;
        Vector4 to4(to[0], to[1], to[2], 1.f);
        to4 = view_->GetViewData().view_matrix * to4;

        from = {from4[0], from4[1], from4[2]};
        to = {to4[0], to4[1], to4[2]};

        for (const Plane &clipping_plane : camera_->clipping_planes_) {
            const float from_distance = clipping_plane.DistanceTo(from);
            const float to_distance = clipping_plane.DistanceTo(to);

            const bool from_outside = from_distance < 0;
            const bool to_outside = to_distance < 0;

            if (from_outside || to_outside) {
                if (from_outside && to_outside)
                    return; // The line is totally outside the plane. Skip.

                Plane::Intersection intersection = clipping_plane.Intersect(from, to);
                assert(intersection.exists);

                if (from_outside)
                    from = intersection.point;
                else
                    to = intersection.point;
            }
        }

        draw_list_.AddLine(to_screen(from), to_screen(to), color);
    };

    view_->UpdateMatrices();

//    draw_line(Vector3(0, 0, -1), Vector3(0, 0, 1), Color::Black());
//    return;

    {
        static const Vector2 net_corners[2] = {
                {-10.f, 0.1f},
                {10.f,  10.1f}
        };
        static const float net_y = 0.f;
        static const size_t net_shares = 100;
        static const Vector2 net_share_step = (net_corners[1] - net_corners[0]) / net_shares;
        static const Color net_color = Color::Green();

        // Vertical lines
        for (size_t line = 0; line <= net_shares; line++) {
            const float x = net_corners[0][0] + net_share_step[0] * static_cast<float>(line);

            const Vector3 from = Vector3(x, net_y, net_corners[0][1]);
            const Vector3 to = Vector3(x, net_y, net_corners[1][1]);

            draw_line(from, to, net_color);
        }

        // Horizontal lines
        for (size_t line = 0; line <= net_shares; line++) {
            const float z = net_corners[0][1] + net_share_step[1] * static_cast<float>(line);

            const Vector3 from = Vector3(net_corners[0][0], net_y, z);
            const Vector3 to = Vector3(net_corners[1][0], net_y, z);

            draw_line(from, to, net_color);
        }
    }

    static const struct {
        Vector3 pos;
        Vector3 pos2;
        Color col;
    } lines[] = {
            {{0.5, 0, 1}, {1,   1,   5}, Color::Black()},
            {{0.5, 0, 1}, {0.5, 0.2, 1}, Color::Red()}
    };

    for (const auto &p : lines)
        draw_line(p.pos, p.pos2, p.col);
}

draw::DrawList *Engine::GetDrawList() {
    return &draw_list_;
}

std::shared_ptr<Camera> Engine::GetActiveCamera() const {
    return camera_;
}

void Engine::AttachController(const std::shared_ptr<Controller> &controller) {
    controllers_.push_back(controller);
    controller->OnAttach(this);
}

void Engine::Update(float ts) {
    for (const std::shared_ptr<Controller> &controller : controllers_)
        controller->Update(ts);
}