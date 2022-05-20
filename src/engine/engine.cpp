#include <cassert>
#include <unordered_map>

#include "engine.h"
#include "math/graphics_utils.h"
#include "math/math.h"
#include "math/frustum.h"

void Engine::Initialize(const ViewPort &viewport) {
    auto camera = std::make_shared<Camera>();
    camera->Initialize(CameraInitializationParameters{
            .aspect_ratio = viewport.GetAspectRatio(),
            .world = std::weak_ptr<World>()
    });

    view_ = std::make_unique<View>();
    view_->SetViewPort(viewport);
    view_->SetCamera(camera);

    screen_space_matrix_ = CreateScreenSpaceMatrix(Vector2(viewport.width, viewport.height));
}

extern std::unordered_map<std::string, CameraInfo> *cameras; // TODO: remove it

void Engine::Draw() {
    draw_list_.Clear();

    view_->UpdateMatrices();

    const auto to_screen = [&](const Vector3 &world_pos) -> Vector2 {
        Vector4 res(world_pos[0], world_pos[1], world_pos[2], 1.f);
        res = view_->GetViewData().view_projection_matrix * res;

        assert(res[3] > 0 && "W value must be greater than zero");

        res /= res[3];

        assert(math::IsInRange(res[0], -1.0f, 1.0f, math::kLargeEpsilon) && "X value is not in range [-1,1]");
        assert(math::IsInRange(res[1], -1.0f, 1.0f, math::kLargeEpsilon) && "Y value is not in range [-1,1]");
        assert(math::IsInRange(res[2], 0.0f, 1.0f, math::kLargeEpsilon) && "Z value must be normalized");

        res = screen_space_matrix_ * res;

        return Vector2(res[0], res[1]);
    };

    const auto draw_line = [&](Vector3 from, Vector3 to, const Color &color) {
        Frustum frustum;
        frustum.SetFromModelViewProjection(view_->GetViewData().view_projection_matrix);
        frustum.Invert();

        for (const Plane &clipping_plane : frustum.planes_) {
            Plane::Intersection intersection = clipping_plane.IntersectLine(from, to);
            if (intersection.Exists()) {
                if (clipping_plane.IsOutside(from))
                    from = intersection.point;
                else
                    to = intersection.point;
            } else if (clipping_plane.IsOutside(from))
                return; // Both points outside the plane and the frustum. Skip.
        }

        draw_list_.AddLine(to_screen(from), to_screen(to), color);
    };

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

    auto active_camera = GetActiveCamera();

    for (const auto &p : *cameras) {
        const CameraInfo &camera_info = p.second;
        if (!camera_info.show_viewing_frustum)
            continue;

        auto camera = camera_info.camera;

        if (camera == active_camera)
            continue; // It's the current camera.

        Frustum frustum;
        frustum.SetFromModelViewProjection(camera_info.camera->ComputeViewProjectionMatrix());

        std::array<Vector3, Frustum::kCornersCount> corner_points = frustum.ComputeCornerPoints();

        const Color frustum_color = camera_info.viewing_frustum_color;

        draw_line(corner_points[Frustum::kNearBottomLeft], corner_points[Frustum::kNearTopLeft], frustum_color);
        draw_line(corner_points[Frustum::kNearBottomLeft], corner_points[Frustum::kNearBottomRight], frustum_color);
        draw_line(corner_points[Frustum::kNearBottomLeft], corner_points[Frustum::kFarBottomLeft], frustum_color);

        draw_line(corner_points[Frustum::kNearTopRight], corner_points[Frustum::kNearTopLeft], frustum_color);
        draw_line(corner_points[Frustum::kNearTopRight], corner_points[Frustum::kNearBottomRight], frustum_color);
        draw_line(corner_points[Frustum::kNearTopRight], corner_points[Frustum::kFarTopRight], frustum_color);

        draw_line(corner_points[Frustum::kFarBottomRight], corner_points[Frustum::kNearBottomRight], frustum_color);
        draw_line(corner_points[Frustum::kFarBottomRight], corner_points[Frustum::kFarTopRight], frustum_color);
        draw_line(corner_points[Frustum::kFarBottomRight], corner_points[Frustum::kFarBottomLeft], frustum_color);

        draw_line(corner_points[Frustum::kFarTopLeft], corner_points[Frustum::kNearTopLeft], frustum_color);
        draw_line(corner_points[Frustum::kFarTopLeft], corner_points[Frustum::kFarTopRight], frustum_color);
        draw_line(corner_points[Frustum::kFarTopLeft], corner_points[Frustum::kFarBottomLeft], frustum_color);

    }
}

draw::DrawList *Engine::GetDrawList() {
    return &draw_list_;
}

std::shared_ptr<Camera> Engine::GetActiveCamera() const {
    return view_->GetCamera();
}

void Engine::SetActiveCamera(const std::shared_ptr<Camera> &camera) {
    view_->SetCamera(camera);
}

std::shared_ptr<Camera> Engine::CreateCamera() {
    auto camera = std::make_shared<Camera>();
    camera->Initialize(CameraInitializationParameters{
            .aspect_ratio = view_->GetViewPort().GetAspectRatio(),
            .world = std::weak_ptr<World>()
    });
    return camera;
}

void Engine::AttachController(const std::shared_ptr<Controller> &controller) {
    controllers_.push_back(controller);
    controller->OnAttach(this);
}

void Engine::Update(float ts) {
    for (const std::shared_ptr<Controller> &controller : controllers_)
        controller->Update(ts);
}