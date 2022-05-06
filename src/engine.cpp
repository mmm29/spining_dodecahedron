#include <cmath>
#include <cassert>

#include "engine.h"
#include "matrix.h"
#include "graphics_utils.h"

void Engine::Initialize() {
    WindowPreferences window_preferences;

    window_preferences.window_size = sf::Vector2f(1280, 720);
    window_preferences.background_color = sf::Color(255, 255, 255);
    window_preferences.window_title = "Dodecahedron";

    screen_ = std::make_unique<Screen>(window_preferences);

    camera_ = std::make_shared<Camera>();
    camera_->SetPosition(Vector3(0, 0, 0));
    camera_->SetRotation(Vector3(0, 0, 0));

    view_ = std::make_unique<View>();
    view_->SetViewPort(ViewPort(window_preferences.window_size.x, window_preferences.window_size.y));
    view_->SetCamera(camera_);
}

bool Engine::Start() {
    if (!screen_->Open())
        return false;

    Matrix4 screen_matrix = CreateScreenSpaceMatrix(Vector2(screen_->GetSize().x, screen_->GetSize().y));

    while (screen_->IsOpen()) {
        screen_->PollEvents();

        screen_->Clear();

        const auto to_screen = [&](const Vector3 &world_pos) -> Vector2 {
            Vector4 world_pos4(world_pos[0], world_pos[1], world_pos[2], 1.f);
            Vector4 res = screen_matrix * view_->GetViewData().view_projection_matrix * world_pos4;
            res /= res[3];
            return Vector2(res[0], res[1]);
        };

        camera_->SetPosition(camera_->GetPosition() + Vector3(0, 0.0005, 0));
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

                screen_->DrawLine(from, to, net_color);
            }

            // Horizontal lines
            for (size_t line = 0; line <= net_shares; line++) {
                const float z = net_corners[0][1] + net_share_step[1] * static_cast<float>(line);

                const Vector2 from = to_screen(Vector3(net_corners[0][0], net_y, z));
                const Vector2 to = to_screen(Vector3(net_corners[1][0], net_y, z));

                screen_->DrawLine(from, to, net_color);
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
            screen_->DrawLine(to_screen(p.pos), to_screen(p.pos2), p.col);

        screen_->DrawTriangle(Triangle(Vector2(200, 50), Vector2(100, 150), Vector2(300, 150), Color::Blue(), 1));

        screen_->Display();
    }

    screen_->Close();

    return true;
}