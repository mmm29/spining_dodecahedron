#include <cassert>
#include <unordered_map>

#include "engine.h"
#include "math/graphics_utils.h"
#include "math/frustum.h"
#include "render/renderer_2d.h"

void Engine::Initialize(const ViewPort &viewport, std::shared_ptr<render::Renderer> &renderer) {
    renderer_ = renderer;

    world_ = std::make_shared<World>();

    auto camera = std::make_shared<Camera>();
    camera->Initialize(CameraInitializationParameters{
            .aspect_ratio = viewport.GetAspectRatio(),
            .world = world_
    });

    view_ = std::make_unique<View>();
    view_->SetViewPort(viewport);
    view_->SetCamera(camera);

    screen_space_matrix_ = CreateScreenSpaceMatrix(Vector2(viewport.width, viewport.height));
}

extern std::unordered_map<std::string, CameraInfo> *cameras; // TODO: remove it

void Engine::Draw() {
    view_->UpdateMatrices();

    render::Renderer2D renderer(renderer_.get());

    Frustum frustum;
    frustum.SetFromModelViewProjection(view_->GetViewData().view_projection_matrix);
    frustum.Invert();

    const auto to_screen = [&](const Vector3 &world_pos) -> Vector2 {
        Vector4 point = view_->GetViewData().view_projection_matrix * world_pos.AsVec4();

        assert(point[3] > 0 && "W value must be greater than zero");

        point /= point[3];

        assert(math::IsInRange(point[0], -1.0f, 1.0f, math::kLargeEpsilon) && "X value is not in range [-1,1]");
        assert(math::IsInRange(point[1], -1.0f, 1.0f, math::kLargeEpsilon) && "Y value is not in range [-1,1]");
        assert(math::IsInRange(point[2], 0.0f, 1.0f, math::kLargeEpsilon) && "Z value must be normalized");

        point = screen_space_matrix_ * point;

        return point.AsVec2();
    };

    const auto draw_line = [&](Vector3 from, Vector3 to, const Color &color) {
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

        renderer.DrawLine(to_screen(from), to_screen(to), color);
    };

    auto draw_triangle = [&](const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Color &color0) {
        Vector3 triangle_normal = (p2 - p1).Cross(p3 - p1);
        triangle_normal.Normalize();

        const Vector3 triangle_center = (p1 + p2 + p3) / 3;
        Vector3 direction_to_triangle = triangle_center - view_->GetCamera()->GetPosition();
        direction_to_triangle.Normalize(); // TODO: Is it necessary?

        const float triangle_dot = triangle_normal.Dot(direction_to_triangle);
        if (triangle_dot > 0)
            return;

        Color color(static_cast<uint8_t>(static_cast<float>(color0.r) * (0.7f + 0.3f * std::abs(triangle_dot))),
                    static_cast<uint8_t>(static_cast<float>(color0.g) * (0.7f + 0.3f * std::abs(triangle_dot))),
                    static_cast<uint8_t>(static_cast<float>(color0.b) * (0.7f + 0.3f * std::abs(triangle_dot))),
                    color0.a);

        std::list<std::array<Vector3, 3>> triangles;
        triangles.emplace_back(std::array<Vector3, 3>{p1, p2, p3});

        for (const Plane &clipping_plane : frustum.planes_) {
            for (auto it = triangles.begin(); it != triangles.end();) {
                std::array<Vector3, 3> &triangle_points = *it;

                std::vector<Vector3> inside_points;
                std::vector<Vector3> outside_points;

                for (const Vector3 &point : triangle_points) {
                    if (clipping_plane.IsOutside(point))
                        outside_points.push_back(point);
                    else
                        inside_points.push_back(point);
                }

                assert(inside_points.size() + outside_points.size() == 3);

                if (inside_points.empty()) {
                    it = triangles.erase(it);
                    continue;
                }

                if (outside_points.empty()) {
                    ++it;
                    continue;
                }

                if (inside_points.size() == 1) {
                    Plane::Intersection intersection1 = clipping_plane.IntersectLine(inside_points[0],
                                                                                     outside_points[0]);
                    Plane::Intersection intersection2 = clipping_plane.IntersectLine(inside_points[0],
                                                                                     outside_points[1]);

                    assert(intersection1.Exists());
                    assert(intersection2.Exists());

                    triangle_points = {inside_points[0], intersection1.Point(), intersection2.Point()};
                } else {
                    assert(inside_points.size() == 2);

                    Plane::Intersection intersection1 = clipping_plane.IntersectLine(inside_points[0],
                                                                                     outside_points[0]);
                    Plane::Intersection intersection2 = clipping_plane.IntersectLine(inside_points[1],
                                                                                     outside_points[0]);
                    assert(intersection1.Exists());
                    assert(intersection2.Exists());

                    triangle_points = {inside_points[0], intersection1.Point(), inside_points[1]};
                    triangles.emplace(it, std::array<Vector3, 3>{intersection1.Point(),
                                                                 intersection2.Point(), inside_points[1]});
                }
                ++it;
            }
        }

        for (const auto &triangle : triangles) {
            std::array<Vector2, 3> screen_pos = {to_screen(triangle[0]),
                                                 to_screen(triangle[1]),
                                                 to_screen(triangle[2])};

            renderer.DrawTriangle(to_screen(triangle[0]),
                                  to_screen(triangle[1]),
                                  to_screen(triangle[2]),
                                  color);

            renderer.DrawLine(screen_pos[0], screen_pos[1], Color::Blue());
            renderer.DrawLine(screen_pos[0], screen_pos[2], Color::Blue());
            renderer.DrawLine(screen_pos[1], screen_pos[2], Color::Blue());
        }
    };

    {
        auto bodies = world_->ListObjects();

        for (const auto &rigid_body : bodies) {
            if (!rigid_body->IsVisible())
                continue;

            const Matrix4 model_matrix = rigid_body->GetModelMatrix();
            const auto &mesh = rigid_body->GetMesh();

            const std::vector<Mesh::Vertex> &vertices = mesh->GetVertices();
            const std::vector<Mesh::Face> &faces = mesh->GetFaces();

            for (const Mesh::Face &face : faces) {
                std::array<Vector3, 3> position;

                for (uint32_t i = 0; i < 3; i++)
                    position[i] = (model_matrix * vertices[face.indices[i]].position.AsVec4()).AsVec3();

                draw_triangle(position[0], position[1], position[2], Color(0xFF, 0xD3, 0xC9, 0xFF));
            }
        }
    }

    draw_triangle(Vector3(-5, -5, -5), Vector3(-0, 0, 0), Vector3(-5, -5, 5), Color::Red());

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

std::shared_ptr<World> Engine::GetWorld() const {
    return world_;
}